#include "lora_driver.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>

#include "esp_attr.h"

#include "config.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logger.h"
#include "lora_registers.h"

using lora::RadioMode;
using lora::RadioResult;

namespace {
constexpr size_t SPI_CHUNK_TOTAL = 64;
constexpr size_t SPI_CHUNK_DATA = SPI_CHUNK_TOTAL - 1;
constexpr uint32_t MID_BAND_THRESHOLD_HZ = 525'000'000;
constexpr uint32_t RESET_LOW_MS = 2;
constexpr uint32_t RESET_BOOT_MS = 10;

inline void delayMs(uint32_t ms) {
    TickType_t ticks = pdMS_TO_TICKS(ms);
    if (ticks == 0) {
        ticks = 1;
    }
    vTaskDelay(ticks);
}
}

Sx1276Radio::Sx1276Radio()
    : spiDevice_(nullptr),
      settings_(lora::defaultSettings()),
      spiInitialized_(false),
      initialized_(false),
      dio0InterruptCount_(0),
      dio1InterruptCount_(0) {}

RadioResult Sx1276Radio::initializeSpi() {
    if (spiInitialized_) {
        return RadioResult::Ok;
    }

    spi_bus_config_t busConfig{};
    busConfig.mosi_io_num = appcfg::LORA_PIN_MOSI;
    busConfig.miso_io_num = appcfg::LORA_PIN_MISO;
    busConfig.sclk_io_num = appcfg::LORA_PIN_SCLK;
    busConfig.quadwp_io_num = -1;
    busConfig.quadhd_io_num = -1;
    busConfig.data4_io_num = -1;
    busConfig.data5_io_num = -1;
    busConfig.data6_io_num = -1;
    busConfig.data7_io_num = -1;
    busConfig.max_transfer_sz = SPI_CHUNK_TOTAL;

    esp_err_t err = spi_bus_initialize(appcfg::LORA_SPI_HOST, &busConfig, SPI_DMA_DISABLED);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        LOGE_LORA("SPI bus init failed: %s", esp_err_to_name(err));
        return RadioResult::SpiError;
    }

    spi_device_interface_config_t deviceConfig{};
    deviceConfig.clock_speed_hz = appcfg::LORA_SPI_CLOCK_HZ;
    deviceConfig.mode = 0;
    deviceConfig.spics_io_num = appcfg::LORA_PIN_NSS;
    deviceConfig.queue_size = 1;

    err = spi_bus_add_device(appcfg::LORA_SPI_HOST, &deviceConfig, &spiDevice_);
    if (err != ESP_OK) {
        LOGE_LORA("SPI device add failed: %s", esp_err_to_name(err));
        return RadioResult::SpiError;
    }

    spiInitialized_ = true;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::initializeGpio() {
    gpio_config_t outputConfig{};
    outputConfig.pin_bit_mask = (1ULL << appcfg::LORA_PIN_RST);
    outputConfig.mode = GPIO_MODE_OUTPUT;
    outputConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    outputConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    outputConfig.intr_type = GPIO_INTR_DISABLE;
    if (gpio_config(&outputConfig) != ESP_OK) {
        return RadioResult::HardwareError;
    }

    gpio_config_t inputConfig{};
    inputConfig.pin_bit_mask = (1ULL << appcfg::LORA_PIN_DIO0) |
                               (1ULL << appcfg::LORA_PIN_DIO1);
    inputConfig.mode = GPIO_MODE_INPUT;
    inputConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    inputConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    inputConfig.intr_type = GPIO_INTR_POSEDGE;
    if (gpio_config(&inputConfig) != ESP_OK) {
        return RadioResult::HardwareError;
    }

    esp_err_t err = gpio_install_isr_service(0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        LOGE_LORA("GPIO ISR service failed: %s", esp_err_to_name(err));
        return RadioResult::HardwareError;
    }

    err = gpio_isr_handler_add(appcfg::LORA_PIN_DIO0, &Sx1276Radio::handleDio0Isr, this);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        LOGW_LORA("DIO0 ISR not installed: %s", esp_err_to_name(err));
    }

    err = gpio_isr_handler_add(appcfg::LORA_PIN_DIO1, &Sx1276Radio::handleDio1Isr, this);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        LOGW_LORA("DIO1 ISR not installed: %s", esp_err_to_name(err));
    }

    return RadioResult::Ok;
}

void IRAM_ATTR Sx1276Radio::handleDio0Isr(void *arg) {
    auto *radio = static_cast<Sx1276Radio *>(arg);
    radio->dio0InterruptCount_++;
}

void IRAM_ATTR Sx1276Radio::handleDio1Isr(void *arg) {
    auto *radio = static_cast<Sx1276Radio *>(arg);
    radio->dio1InterruptCount_++;
}

RadioResult Sx1276Radio::begin(const lora::RadioSettings &settings) {
    if (!validateSettings(settings)) {
        return RadioResult::InvalidArgument;
    }

    RadioResult result = initializeGpio();
    if (result != RadioResult::Ok) {
        return result;
    }
    result = initializeSpi();
    if (result != RadioResult::Ok) {
        return result;
    }
    result = reset();
    if (result != RadioResult::Ok) {
        return result;
    }

    const uint8_t version = getVersion();
    LOGI_LORA("SX1276 RegVersion=0x%02X", version);
    if (version != appcfg::SX1276_EXPECTED_VERSION) {
        LOGE_LORA("Unexpected chip version. Expected 0x%02X", appcfg::SX1276_EXPECTED_VERSION);
        return RadioResult::VersionMismatch;
    }

    // LongRangeMode can only be changed while the radio is in sleep.
    writeRegister(sx1276::REG_OP_MODE, sx1276::OPMODE_LONG_RANGE);
    delayMs(2);

    settings_ = settings;
    result = applySettings(settings_);
    if (result != RadioResult::Ok) {
        return result;
    }

    writeRegister(sx1276::REG_FIFO_TX_BASE_ADDR, 0x00);
    writeRegister(sx1276::REG_FIFO_RX_BASE_ADDR, 0x00);
    writeRegister(sx1276::REG_MAX_PAYLOAD_LENGTH, 0xFF);
    configureLna();
    clearIrqFlags();
    standby();

    initialized_ = true;
    LOGI_LORA("SX1276 initialized at %lu Hz", static_cast<unsigned long>(settings_.frequencyHz));
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::reset() {
    gpio_set_level(appcfg::LORA_PIN_RST, 1);
    delayMs(1);
    gpio_set_level(appcfg::LORA_PIN_RST, 0);
    delayMs(RESET_LOW_MS);
    gpio_set_level(appcfg::LORA_PIN_RST, 1);
    delayMs(RESET_BOOT_MS);
    initialized_ = false;
    return RadioResult::Ok;
}

bool Sx1276Radio::validateSettings(const lora::RadioSettings &settings) const {
    if (settings.frequencyHz < appcfg::MIN_FREQUENCY_HZ || settings.frequencyHz > appcfg::MAX_FREQUENCY_HZ) {
        return false;
    }
    if (settings.bandwidthCode > 9 || lora::bandwidthToHz(settings.bandwidthCode) == 0) {
        return false;
    }
    if (settings.spreadingFactor < 6 || settings.spreadingFactor > 12) {
        return false;
    }
    if (settings.codingRate < 1 || settings.codingRate > 4) {
        return false;
    }
    if (settings.preambleSymbols < 6) {
        return false;
    }
    if (settings.rxSymbolTimeout == 0 || settings.rxSymbolTimeout > 1023) {
        return false;
    }
    if (settings.implicitPayloadLength == 0) {
        return false;
    }
    if (settings.paBoost) {
        if (settings.txPowerDbm < 2 || settings.txPowerDbm > 20) {
            return false;
        }
    } else if (settings.txPowerDbm < -4 || settings.txPowerDbm > 15) {
        return false;
    }
    return true;
}

RadioResult Sx1276Radio::applySettings(const lora::RadioSettings &settings) {
    if (!validateSettings(settings)) {
        return RadioResult::InvalidArgument;
    }

    settings_ = settings;
    if (settings_.spreadingFactor == 6 && !settings_.implicitHeader) {
        LOGW_LORA("SF6 requires implicit header. Enabling implicit mode automatically.");
        settings_.implicitHeader = true;
    }

    RadioResult result = standby();
    if (result != RadioResult::Ok && spiInitialized_) {
        // During the first begin(), standby is still valid even before initialized_ becomes true.
        result = setMode(RadioMode::Standby);
    }
    if (result != RadioResult::Ok) return result;

    if ((result = setFrequency(settings_.frequencyHz)) != RadioResult::Ok) return result;
    if ((result = setBandwidth(settings_.bandwidthCode)) != RadioResult::Ok) return result;
    if ((result = setCodingRate(settings_.codingRate)) != RadioResult::Ok) return result;
    if ((result = setSpreadingFactor(settings_.spreadingFactor)) != RadioResult::Ok) return result;
    if ((result = setHeaderMode(settings_.implicitHeader)) != RadioResult::Ok) return result;
    if ((result = setCrc(settings_.crcEnabled)) != RadioResult::Ok) return result;
    if ((result = setPreamble(settings_.preambleSymbols)) != RadioResult::Ok) return result;
    if ((result = setSyncWord(settings_.syncWord)) != RadioResult::Ok) return result;
    if ((result = setIqInverted(settings_.iqInverted)) != RadioResult::Ok) return result;
    if ((result = setRxSymbolTimeout(settings_.rxSymbolTimeout)) != RadioResult::Ok) return result;
    if ((result = setTxPower(settings_.txPowerDbm, settings_.paBoost)) != RadioResult::Ok) return result;
    if ((result = setImplicitPayloadLength(settings_.implicitPayloadLength)) != RadioResult::Ok) return result;

    uint8_t modem3 = 0;
    readRegister(sx1276::REG_MODEM_CONFIG_3, modem3);
    modem3 |= 0x04; // AGC auto on
    writeRegister(sx1276::REG_MODEM_CONFIG_3, modem3);
    updateLowDataRateOptimize();
    updateHighBandwidthOptimization();

    return RadioResult::Ok;
}

RadioResult Sx1276Radio::transfer(const uint8_t *tx, uint8_t *rx, size_t length) {
    if (!spiDevice_ || length == 0 || length > SPI_CHUNK_TOTAL) {
        return RadioResult::InvalidArgument;
    }

    spi_transaction_t transaction{};
    transaction.length = length * 8;
    transaction.tx_buffer = tx;
    transaction.rx_buffer = rx;
    const esp_err_t err = spi_device_transmit(spiDevice_, &transaction);
    return err == ESP_OK ? RadioResult::Ok : RadioResult::SpiError;
}

RadioResult Sx1276Radio::readRegister(uint8_t address, uint8_t &value) {
    uint8_t tx[2] = {static_cast<uint8_t>(address & 0x7F), 0x00};
    uint8_t rx[2] = {0, 0};
    const RadioResult result = transfer(tx, rx, sizeof(tx));
    if (result == RadioResult::Ok) {
        value = rx[1];
    }
    return result;
}

RadioResult Sx1276Radio::writeRegister(uint8_t address, uint8_t value) {
    uint8_t tx[2] = {static_cast<uint8_t>(address | 0x80), value};
    uint8_t rx[2] = {0, 0};
    return transfer(tx, rx, sizeof(tx));
}

RadioResult Sx1276Radio::readBuffer(uint8_t address, uint8_t *data, size_t length) {
    if (!data || length == 0) return RadioResult::InvalidArgument;

    size_t offset = 0;
    while (offset < length) {
        const size_t chunk = std::min(SPI_CHUNK_DATA, length - offset);
        uint8_t tx[SPI_CHUNK_TOTAL]{};
        uint8_t rx[SPI_CHUNK_TOTAL]{};
        tx[0] = static_cast<uint8_t>(address & 0x7F);
        RadioResult result = transfer(tx, rx, chunk + 1);
        if (result != RadioResult::Ok) return result;
        std::memcpy(data + offset, rx + 1, chunk);
        offset += chunk;
    }
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::writeBuffer(uint8_t address, const uint8_t *data, size_t length) {
    if (!data || length == 0) return RadioResult::InvalidArgument;

    size_t offset = 0;
    while (offset < length) {
        const size_t chunk = std::min(SPI_CHUNK_DATA, length - offset);
        uint8_t tx[SPI_CHUNK_TOTAL]{};
        uint8_t rx[SPI_CHUNK_TOTAL]{};
        tx[0] = static_cast<uint8_t>(address | 0x80);
        std::memcpy(tx + 1, data + offset, chunk);
        RadioResult result = transfer(tx, rx, chunk + 1);
        if (result != RadioResult::Ok) return result;
        offset += chunk;
    }
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setMode(RadioMode mode) {
    const bool lowFrequency = settings_.frequencyHz < MID_BAND_THRESHOLD_HZ;
    const uint8_t opMode = sx1276::OPMODE_LONG_RANGE |
                           (lowFrequency ? sx1276::OPMODE_LOW_FREQUENCY : 0) |
                           static_cast<uint8_t>(mode);
    return writeRegister(sx1276::REG_OP_MODE, opMode);
}

RadioResult Sx1276Radio::sleep() {
    return setMode(RadioMode::Sleep);
}

RadioResult Sx1276Radio::standby() {
    return setMode(RadioMode::Standby);
}

RadioResult Sx1276Radio::setFrequency(uint32_t frequencyHz) {
    if (frequencyHz < appcfg::MIN_FREQUENCY_HZ || frequencyHz > appcfg::MAX_FREQUENCY_HZ) {
        return RadioResult::InvalidArgument;
    }
    settings_.frequencyHz = frequencyHz;
    const uint64_t frf = (static_cast<uint64_t>(frequencyHz) << 19U) / appcfg::LORA_XTAL_HZ;
    if (writeRegister(sx1276::REG_FRF_MSB, static_cast<uint8_t>(frf >> 16U)) != RadioResult::Ok) return RadioResult::SpiError;
    if (writeRegister(sx1276::REG_FRF_MID, static_cast<uint8_t>(frf >> 8U)) != RadioResult::Ok) return RadioResult::SpiError;
    if (writeRegister(sx1276::REG_FRF_LSB, static_cast<uint8_t>(frf)) != RadioResult::Ok) return RadioResult::SpiError;

    uint8_t currentMode = 0;
    readRegister(sx1276::REG_OP_MODE, currentMode);
    currentMode &= static_cast<uint8_t>(~sx1276::OPMODE_LOW_FREQUENCY);
    if (frequencyHz < MID_BAND_THRESHOLD_HZ) currentMode |= sx1276::OPMODE_LOW_FREQUENCY;
    currentMode |= sx1276::OPMODE_LONG_RANGE;
    writeRegister(sx1276::REG_OP_MODE, currentMode);
    updateHighBandwidthOptimization();
    return RadioResult::Ok;
}

uint32_t Sx1276Radio::getFrequencyHz() {
    uint8_t msb = 0, mid = 0, lsb = 0;
    if (readRegister(sx1276::REG_FRF_MSB, msb) != RadioResult::Ok ||
        readRegister(sx1276::REG_FRF_MID, mid) != RadioResult::Ok ||
        readRegister(sx1276::REG_FRF_LSB, lsb) != RadioResult::Ok) {
        return 0;
    }
    const uint32_t frf = (static_cast<uint32_t>(msb) << 16U) |
                         (static_cast<uint32_t>(mid) << 8U) | lsb;
    return static_cast<uint32_t>((static_cast<uint64_t>(frf) * appcfg::LORA_XTAL_HZ) >> 19U);
}

RadioResult Sx1276Radio::setBandwidth(uint8_t bandwidthCode) {
    if (bandwidthCode > 9) return RadioResult::InvalidArgument;
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_1, reg) != RadioResult::Ok) return RadioResult::SpiError;
    reg = static_cast<uint8_t>((reg & 0x0F) | (bandwidthCode << 4U));
    if (writeRegister(sx1276::REG_MODEM_CONFIG_1, reg) != RadioResult::Ok) return RadioResult::SpiError;
    settings_.bandwidthCode = bandwidthCode;
    updateLowDataRateOptimize();
    updateHighBandwidthOptimization();
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setCodingRate(uint8_t codingRate) {
    if (codingRate < 1 || codingRate > 4) return RadioResult::InvalidArgument;
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_1, reg) != RadioResult::Ok) return RadioResult::SpiError;
    reg = static_cast<uint8_t>((reg & 0xF1) | (codingRate << 1U));
    if (writeRegister(sx1276::REG_MODEM_CONFIG_1, reg) != RadioResult::Ok) return RadioResult::SpiError;
    settings_.codingRate = codingRate;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setSpreadingFactor(uint8_t spreadingFactor) {
    if (spreadingFactor < 6 || spreadingFactor > 12) return RadioResult::InvalidArgument;

    uint8_t modem2 = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_2, modem2) != RadioResult::Ok) return RadioResult::SpiError;
    modem2 = static_cast<uint8_t>((modem2 & 0x0F) | (spreadingFactor << 4U));
    if (writeRegister(sx1276::REG_MODEM_CONFIG_2, modem2) != RadioResult::Ok) return RadioResult::SpiError;

    uint8_t detectOptimize = 0;
    readRegister(sx1276::REG_DETECT_OPTIMIZE, detectOptimize);
    detectOptimize &= 0xF8;
    if (spreadingFactor == 6) {
        detectOptimize |= sx1276::DETECT_OPTIMIZE_SF6;
        writeRegister(sx1276::REG_DETECTION_THRESHOLD, sx1276::DETECTION_THRESHOLD_SF6);
        settings_.implicitHeader = true;
        setHeaderMode(true);
    } else {
        detectOptimize |= sx1276::DETECT_OPTIMIZE_SF7_TO_SF12;
        writeRegister(sx1276::REG_DETECTION_THRESHOLD, sx1276::DETECTION_THRESHOLD_SF7_TO_SF12);
    }
    writeRegister(sx1276::REG_DETECT_OPTIMIZE, detectOptimize);

    settings_.spreadingFactor = spreadingFactor;
    updateLowDataRateOptimize();
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setHeaderMode(bool implicitHeader) {
    if (settings_.spreadingFactor == 6 && !implicitHeader) {
        return RadioResult::InvalidArgument;
    }
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_1, reg) != RadioResult::Ok) return RadioResult::SpiError;
    if (implicitHeader) reg |= 0x01;
    else reg &= 0xFE;
    if (writeRegister(sx1276::REG_MODEM_CONFIG_1, reg) != RadioResult::Ok) return RadioResult::SpiError;
    settings_.implicitHeader = implicitHeader;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setCrc(bool enabled) {
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_2, reg) != RadioResult::Ok) return RadioResult::SpiError;
    if (enabled) reg |= 0x04;
    else reg &= 0xFB;
    if (writeRegister(sx1276::REG_MODEM_CONFIG_2, reg) != RadioResult::Ok) return RadioResult::SpiError;
    settings_.crcEnabled = enabled;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setPreamble(uint16_t symbols) {
    if (symbols < 6) return RadioResult::InvalidArgument;
    if (writeRegister(sx1276::REG_PREAMBLE_MSB, static_cast<uint8_t>(symbols >> 8U)) != RadioResult::Ok) return RadioResult::SpiError;
    if (writeRegister(sx1276::REG_PREAMBLE_LSB, static_cast<uint8_t>(symbols)) != RadioResult::Ok) return RadioResult::SpiError;
    settings_.preambleSymbols = symbols;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setSyncWord(uint8_t syncWord) {
    if (writeRegister(sx1276::REG_SYNC_WORD, syncWord) != RadioResult::Ok) return RadioResult::SpiError;
    settings_.syncWord = syncWord;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setRxSymbolTimeout(uint16_t symbols) {
    if (symbols == 0 || symbols > 1023) return RadioResult::InvalidArgument;
    uint8_t modem2 = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_2, modem2) != RadioResult::Ok) return RadioResult::SpiError;
    modem2 = static_cast<uint8_t>((modem2 & 0xFC) | ((symbols >> 8U) & 0x03));
    if (writeRegister(sx1276::REG_MODEM_CONFIG_2, modem2) != RadioResult::Ok) return RadioResult::SpiError;
    if (writeRegister(sx1276::REG_SYMB_TIMEOUT_LSB, static_cast<uint8_t>(symbols)) != RadioResult::Ok) return RadioResult::SpiError;
    settings_.rxSymbolTimeout = symbols;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setImplicitPayloadLength(uint8_t length) {
    if (length == 0) return RadioResult::InvalidArgument;
    settings_.implicitPayloadLength = length;
    if (settings_.implicitHeader) {
        return writeRegister(sx1276::REG_PAYLOAD_LENGTH, length);
    }
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::setIqInverted(bool inverted) {
    settings_.iqInverted = inverted;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::configureIqForTx() {
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_INVERT_IQ, reg) != RadioResult::Ok) return RadioResult::SpiError;
    reg &= sx1276::INVERT_IQ_TX_MASK;
    reg &= sx1276::INVERT_IQ_RX_MASK;
    reg |= sx1276::INVERT_IQ_RX_OFF;
    reg |= settings_.iqInverted ? sx1276::INVERT_IQ_TX_ON : sx1276::INVERT_IQ_TX_OFF;
    if (writeRegister(sx1276::REG_INVERT_IQ, reg) != RadioResult::Ok) return RadioResult::SpiError;
    return writeRegister(sx1276::REG_INVERT_IQ_2,
                         settings_.iqInverted ? sx1276::INVERT_IQ_2_ON : sx1276::INVERT_IQ_2_OFF);
}

RadioResult Sx1276Radio::configureIqForRx() {
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_INVERT_IQ, reg) != RadioResult::Ok) return RadioResult::SpiError;
    reg &= sx1276::INVERT_IQ_TX_MASK;
    reg &= sx1276::INVERT_IQ_RX_MASK;
    reg |= settings_.iqInverted ? sx1276::INVERT_IQ_RX_ON : sx1276::INVERT_IQ_RX_OFF;
    reg |= sx1276::INVERT_IQ_TX_OFF;
    if (writeRegister(sx1276::REG_INVERT_IQ, reg) != RadioResult::Ok) return RadioResult::SpiError;
    return writeRegister(sx1276::REG_INVERT_IQ_2,
                         settings_.iqInverted ? sx1276::INVERT_IQ_2_ON : sx1276::INVERT_IQ_2_OFF);
}

RadioResult Sx1276Radio::setTxPower(int8_t powerDbm, bool paBoost) {
    if (paBoost) {
        if (powerDbm < 2 || powerDbm > 20) return RadioResult::InvalidArgument;
        if (powerDbm > 17) {
            // +20 dBm path. Values 18/19/20 map to the high-power DAC with reduced OutputPower.
            writeRegister(sx1276::REG_PA_DAC, sx1276::PA_DAC_20_DBM);
            const uint8_t ocpTrim = static_cast<uint8_t>((140 + 30) / 10);
            writeRegister(sx1276::REG_OCP, static_cast<uint8_t>(0x20 | (ocpTrim & 0x1F)));
            const uint8_t outputPower = static_cast<uint8_t>(std::clamp<int>(powerDbm - 5, 0, 15));
            writeRegister(sx1276::REG_PA_CONFIG, static_cast<uint8_t>(sx1276::PA_BOOST | 0x70 | outputPower));
        } else {
            writeRegister(sx1276::REG_PA_DAC, sx1276::PA_DAC_DEFAULT);
            writeRegister(sx1276::REG_OCP, 0x2B); // OCP on, ~100 mA
            const uint8_t outputPower = static_cast<uint8_t>(powerDbm - 2);
            writeRegister(sx1276::REG_PA_CONFIG, static_cast<uint8_t>(sx1276::PA_BOOST | 0x70 | outputPower));
        }
    } else {
        if (powerDbm < -4 || powerDbm > 15) return RadioResult::InvalidArgument;
        writeRegister(sx1276::REG_PA_DAC, sx1276::PA_DAC_DEFAULT);
        const uint8_t maxPower = 7;
        const int pmaxTenths = 108 + 6 * maxPower;
        const int output = std::clamp<int>((powerDbm * 10 - pmaxTenths + 150) / 10, 0, 15);
        writeRegister(sx1276::REG_PA_CONFIG, static_cast<uint8_t>((maxPower << 4U) | output));
    }

    settings_.txPowerDbm = powerDbm;
    settings_.paBoost = paBoost;
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::configureLna() {
    uint8_t lna = 0;
    if (readRegister(sx1276::REG_LNA, lna) != RadioResult::Ok) return RadioResult::SpiError;
    lna |= 0x03;
    return writeRegister(sx1276::REG_LNA, lna);
}

void Sx1276Radio::updateLowDataRateOptimize() {
    const uint32_t bandwidthHz = lora::bandwidthToHz(settings_.bandwidthCode);
    if (bandwidthHz == 0) return;
    const double symbolMs = (static_cast<double>(1UL << settings_.spreadingFactor) * 1000.0) /
                            static_cast<double>(bandwidthHz);
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_3, reg) != RadioResult::Ok) return;
    reg |= 0x04; // AGC auto
    if (symbolMs > 16.0) reg |= 0x08;
    else reg &= 0xF7;
    writeRegister(sx1276::REG_MODEM_CONFIG_3, reg);
}

void Sx1276Radio::updateHighBandwidthOptimization() {
    if (settings_.bandwidthCode == 9) {
        writeRegister(sx1276::REG_HIGH_BW_OPTIMIZE_1, 0x02);
        writeRegister(sx1276::REG_HIGH_BW_OPTIMIZE_2,
                      settings_.frequencyHz > MID_BAND_THRESHOLD_HZ ? 0x64 : 0x7F);
    } else {
        writeRegister(sx1276::REG_HIGH_BW_OPTIMIZE_1, 0x03);
    }
}

RadioResult Sx1276Radio::clearIrqFlags(uint8_t mask) {
    return writeRegister(sx1276::REG_IRQ_FLAGS, mask);
}

uint8_t Sx1276Radio::getIrqFlags() {
    uint8_t value = 0;
    readRegister(sx1276::REG_IRQ_FLAGS, value);
    return value;
}

RadioResult Sx1276Radio::transmit(const uint8_t *data, size_t length, uint32_t timeoutMs) {
    if (!data || length == 0 || length > appcfg::RADIO_MAX_PAYLOAD) return RadioResult::InvalidArgument;

    standby();
    configureIqForTx();
    setTxContinuousMode(false);

    uint8_t mapping = 0;
    readRegister(sx1276::REG_DIO_MAPPING_1, mapping);
    mapping = static_cast<uint8_t>((mapping & sx1276::DIO0_MASK) | sx1276::DIO0_TX_DONE);
    writeRegister(sx1276::REG_DIO_MAPPING_1, mapping);

    clearIrqFlags();
    writeRegister(sx1276::REG_FIFO_TX_BASE_ADDR, 0x00);
    writeRegister(sx1276::REG_FIFO_ADDR_PTR, 0x00);
    writeRegister(sx1276::REG_PAYLOAD_LENGTH, static_cast<uint8_t>(length));
    if (writeBuffer(sx1276::REG_FIFO, data, length) != RadioResult::Ok) return RadioResult::SpiError;

    setMode(RadioMode::Tx);
    const int64_t startUs = esp_timer_get_time();
    while ((esp_timer_get_time() - startUs) < static_cast<int64_t>(timeoutMs) * 1000LL) {
        const uint8_t flags = getIrqFlags();
        if (flags & sx1276::IRQ_TX_DONE) {
            clearIrqFlags(sx1276::IRQ_TX_DONE);
            standby();
            return RadioResult::Ok;
        }
        delayMs(appcfg::RADIO_POLL_INTERVAL_MS);
    }

    standby();
    return RadioResult::Timeout;
}

RadioResult Sx1276Radio::configureRxErrata(uint32_t &originalFrequencyHz) {
    originalFrequencyHz = settings_.frequencyHz;
    uint8_t detect = 0;
    readRegister(sx1276::REG_DETECT_OPTIMIZE, detect);

    if (settings_.bandwidthCode < 9) {
        writeRegister(sx1276::REG_DETECT_OPTIMIZE, static_cast<uint8_t>(detect & 0x7F));
        writeRegister(sx1276::REG_IF_FREQ_2, 0x00);
        uint32_t offsetHz = 0;
        uint8_t ifFreq1 = 0x40;
        switch (settings_.bandwidthCode) {
            case 0: ifFreq1 = 0x48; offsetHz = 7'810; break;
            case 1: ifFreq1 = 0x44; offsetHz = 10'420; break;
            case 2: ifFreq1 = 0x44; offsetHz = 15'620; break;
            case 3: ifFreq1 = 0x44; offsetHz = 20'830; break;
            case 4: ifFreq1 = 0x44; offsetHz = 31'250; break;
            case 5: ifFreq1 = 0x44; offsetHz = 41'670; break;
            case 6:
            case 7:
            case 8: ifFreq1 = 0x40; break;
            default: break;
        }
        writeRegister(sx1276::REG_IF_FREQ_1, ifFreq1);
        if (offsetHz != 0) {
            setFrequency(originalFrequencyHz + offsetHz);
            settings_.frequencyHz = originalFrequencyHz;
        }
    } else {
        writeRegister(sx1276::REG_DETECT_OPTIMIZE, static_cast<uint8_t>(detect | 0x80));
    }
    return RadioResult::Ok;
}

void Sx1276Radio::restoreRxFrequency(uint32_t originalFrequencyHz) {
    if (getFrequencyHz() != originalFrequencyHz) {
        setFrequency(originalFrequencyHz);
    }
    settings_.frequencyHz = originalFrequencyHz;
}

RadioResult Sx1276Radio::startReceiveContinuous() {
    standby();
    configureIqForRx();
    uint32_t originalFrequency = settings_.frequencyHz;
    configureRxErrata(originalFrequency);

    uint8_t mapping = 0;
    readRegister(sx1276::REG_DIO_MAPPING_1, mapping);
    mapping = static_cast<uint8_t>((mapping & sx1276::DIO0_MASK) | sx1276::DIO0_RX_DONE);
    writeRegister(sx1276::REG_DIO_MAPPING_1, mapping);

    clearIrqFlags();
    if (settings_.implicitHeader) {
        writeRegister(sx1276::REG_PAYLOAD_LENGTH, settings_.implicitPayloadLength);
    }
    writeRegister(sx1276::REG_FIFO_ADDR_PTR, 0x00);
    return setMode(RadioMode::RxContinuous);
}

RadioResult Sx1276Radio::pollReceivedPacket(uint8_t *buffer, size_t capacity, lora::PacketInfo &info) {
    if (!buffer || capacity == 0) return RadioResult::InvalidArgument;
    const uint8_t flags = getIrqFlags();
    info.irqFlags = flags;

    if (!(flags & sx1276::IRQ_RX_DONE)) {
        return RadioResult::NoPacket;
    }
    if (flags & sx1276::IRQ_PAYLOAD_CRC_ERROR) {
        clearIrqFlags(static_cast<uint8_t>(sx1276::IRQ_RX_DONE | sx1276::IRQ_PAYLOAD_CRC_ERROR));
        return RadioResult::CrcError;
    }

    uint8_t length = 0;
    uint8_t currentAddr = 0;
    readRegister(sx1276::REG_RX_NB_BYTES, length);
    readRegister(sx1276::REG_FIFO_RX_CURRENT_ADDR, currentAddr);
    writeRegister(sx1276::REG_FIFO_ADDR_PTR, currentAddr);

    const size_t readLength = std::min<size_t>(length, capacity);
    if (readLength > 0 && readBuffer(sx1276::REG_FIFO, buffer, readLength) != RadioResult::Ok) {
        return RadioResult::SpiError;
    }

    info.length = readLength;
    info.snrDb = readPacketSnrDb();
    info.rssiDbm = readPacketRssiDbm();
    info.frequencyErrorHz = readFrequencyErrorHz();
    clearIrqFlags(static_cast<uint8_t>(sx1276::IRQ_RX_DONE | sx1276::IRQ_VALID_HEADER));
    return RadioResult::Ok;
}

RadioResult Sx1276Radio::receive(uint8_t *buffer, size_t capacity, lora::PacketInfo &info, uint32_t timeoutMs) {
    if (!buffer || capacity == 0) return RadioResult::InvalidArgument;

    const uint32_t originalFrequency = settings_.frequencyHz;
    RadioResult result = startReceiveContinuous();
    if (result != RadioResult::Ok) return result;

    const int64_t startUs = esp_timer_get_time();
    while ((esp_timer_get_time() - startUs) < static_cast<int64_t>(timeoutMs) * 1000LL) {
        result = pollReceivedPacket(buffer, capacity, info);
        if (result == RadioResult::Ok || result == RadioResult::CrcError) {
            standby();
            restoreRxFrequency(originalFrequency);
            return result;
        }
        delayMs(appcfg::RADIO_POLL_INTERVAL_MS);
    }

    standby();
    restoreRxFrequency(originalFrequency);
    return RadioResult::Timeout;
}

RadioResult Sx1276Radio::performCad(lora::CadResult &result, uint32_t timeoutMs) {
    standby();
    configureIqForRx();
    clearIrqFlags();

    uint8_t mapping = 0;
    readRegister(sx1276::REG_DIO_MAPPING_1, mapping);
    mapping = static_cast<uint8_t>((mapping & sx1276::DIO0_MASK) | sx1276::DIO0_CAD_DONE);
    writeRegister(sx1276::REG_DIO_MAPPING_1, mapping);

    setMode(RadioMode::Cad);
    const int64_t startUs = esp_timer_get_time();
    while ((esp_timer_get_time() - startUs) < static_cast<int64_t>(timeoutMs) * 1000LL) {
        const uint8_t flags = getIrqFlags();
        if (flags & sx1276::IRQ_CAD_DONE) {
            result.completed = true;
            result.detected = (flags & sx1276::IRQ_CAD_DETECTED) != 0;
            result.irqFlags = flags;
            clearIrqFlags(static_cast<uint8_t>(sx1276::IRQ_CAD_DONE | sx1276::IRQ_CAD_DETECTED));
            standby();
            return RadioResult::Ok;
        }
        delayMs(1);
    }
    result.completed = false;
    result.detected = false;
    result.irqFlags = getIrqFlags();
    standby();
    return RadioResult::Timeout;
}

RadioResult Sx1276Radio::runRxSingleTimeoutTest(bool &rxTimeoutSeen, uint32_t timeoutMs) {
    standby();
    configureIqForRx();
    clearIrqFlags();
    uint8_t mapping = 0;
    readRegister(sx1276::REG_DIO_MAPPING_1, mapping);
    mapping = static_cast<uint8_t>((mapping & sx1276::DIO1_MASK) | sx1276::DIO1_RX_TIMEOUT);
    writeRegister(sx1276::REG_DIO_MAPPING_1, mapping);
    setMode(RadioMode::RxSingle);

    const int64_t startUs = esp_timer_get_time();
    rxTimeoutSeen = false;
    while ((esp_timer_get_time() - startUs) < static_cast<int64_t>(timeoutMs) * 1000LL) {
        const uint8_t flags = getIrqFlags();
        if (flags & sx1276::IRQ_RX_TIMEOUT) {
            rxTimeoutSeen = true;
            clearIrqFlags(sx1276::IRQ_RX_TIMEOUT);
            standby();
            return RadioResult::Ok;
        }
        if (flags & sx1276::IRQ_RX_DONE) {
            clearIrqFlags();
            standby();
            return RadioResult::Ok;
        }
        delayMs(1);
    }
    standby();
    return RadioResult::Timeout;
}

int16_t Sx1276Radio::getRssiOffsetDb() const {
    return settings_.frequencyHz < MID_BAND_THRESHOLD_HZ ? -164 : -157;
}

float Sx1276Radio::readCurrentRssiDbm() {
    uint8_t raw = 0;
    if (readRegister(sx1276::REG_RSSI_VALUE, raw) != RadioResult::Ok) return -999.0f;
    return static_cast<float>(getRssiOffsetDb() + raw);
}

float Sx1276Radio::readPacketSnrDb() {
    uint8_t raw = 0;
    if (readRegister(sx1276::REG_PKT_SNR_VALUE, raw) != RadioResult::Ok) return -999.0f;
    const int8_t signedRaw = static_cast<int8_t>(raw);
    return static_cast<float>(signedRaw) / 4.0f;
}

float Sx1276Radio::readPacketRssiDbm() {
    uint8_t raw = 0;
    if (readRegister(sx1276::REG_PKT_RSSI_VALUE, raw) != RadioResult::Ok) return -999.0f;
    const float snr = readPacketSnrDb();
    float rssi = static_cast<float>(getRssiOffsetDb() + raw);
    if (snr < 0.0f) rssi += snr;
    return rssi;
}

float Sx1276Radio::readFrequencyErrorHz() {
    uint8_t msb = 0, mid = 0, lsb = 0;
    if (readRegister(sx1276::REG_FEI_MSB, msb) != RadioResult::Ok ||
        readRegister(sx1276::REG_FEI_MID, mid) != RadioResult::Ok ||
        readRegister(sx1276::REG_FEI_LSB, lsb) != RadioResult::Ok) {
        return 0.0f;
    }
    int32_t raw = (static_cast<int32_t>(msb & 0x0F) << 16) |
                  (static_cast<int32_t>(mid) << 8) | lsb;
    if (msb & 0x08) raw -= (1 << 20);

    const double bandwidthHz = static_cast<double>(lora::bandwidthToHz(settings_.bandwidthCode));
    const double errorHz = static_cast<double>(raw) * (16777216.0 / appcfg::LORA_XTAL_HZ) *
                           (bandwidthHz / 500000.0);
    return static_cast<float>(errorHz);
}

uint8_t Sx1276Radio::readWidebandRssi() {
    uint8_t value = 0;
    readRegister(sx1276::REG_RSSI_WIDEBAND, value);
    return value;
}

RadioResult Sx1276Radio::setTxContinuousMode(bool enabled) {
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_MODEM_CONFIG_2, reg) != RadioResult::Ok) return RadioResult::SpiError;
    if (enabled) reg |= 0x08;
    else reg &= 0xF7;
    return writeRegister(sx1276::REG_MODEM_CONFIG_2, reg);
}

RadioResult Sx1276Radio::setHopPeriod(uint8_t symbols) {
    return writeRegister(sx1276::REG_HOP_PERIOD, symbols);
}

RadioResult Sx1276Radio::setFastHop(bool enabled) {
    uint8_t reg = 0;
    if (readRegister(sx1276::REG_PLL_HOP, reg) != RadioResult::Ok) return RadioResult::SpiError;
    if (enabled) reg |= 0x80;
    else reg &= 0x7F;
    return writeRegister(sx1276::REG_PLL_HOP, reg);
}

uint8_t Sx1276Radio::getVersion() {
    uint8_t version = 0;
    readRegister(sx1276::REG_VERSION, version);
    return version;
}

uint16_t Sx1276Radio::getRxHeaderCount() {
    uint8_t msb = 0, lsb = 0;
    readRegister(sx1276::REG_RX_HEADER_CNT_VALUE_MSB, msb);
    readRegister(sx1276::REG_RX_HEADER_CNT_VALUE_LSB, lsb);
    return static_cast<uint16_t>((msb << 8U) | lsb);
}

uint16_t Sx1276Radio::getRxPacketCount() {
    uint8_t msb = 0, lsb = 0;
    readRegister(sx1276::REG_RX_PACKET_CNT_VALUE_MSB, msb);
    readRegister(sx1276::REG_RX_PACKET_CNT_VALUE_LSB, lsb);
    return static_cast<uint16_t>((msb << 8U) | lsb);
}

uint32_t Sx1276Radio::getDio0InterruptCount() const {
    return dio0InterruptCount_;
}

uint32_t Sx1276Radio::getDio1InterruptCount() const {
    return dio1InterruptCount_;
}

double Sx1276Radio::calculateTimeOnAirMs(size_t payloadLength) const {
    if (payloadLength > appcfg::RADIO_MAX_PAYLOAD) return 0.0;
    const double bw = static_cast<double>(lora::bandwidthToHz(settings_.bandwidthCode));
    if (bw <= 0.0) return 0.0;

    const int sf = settings_.spreadingFactor;
    const int cr = settings_.codingRate;
    const int ih = settings_.implicitHeader ? 1 : 0;
    const int crc = settings_.crcEnabled ? 1 : 0;
    const double symbolSeconds = static_cast<double>(1UL << sf) / bw;
    const int de = symbolSeconds > 0.016 ? 1 : 0;
    const double preambleSymbols = static_cast<double>(settings_.preambleSymbols) + 4.25;
    const double numerator = 8.0 * payloadLength - 4.0 * sf + 28.0 + 16.0 * crc - 20.0 * ih;
    const double denominator = 4.0 * (sf - 2 * de);
    const double payloadTerm = std::max(0.0, std::ceil(numerator / denominator) * (cr + 4));
    const double payloadSymbols = 8.0 + payloadTerm;
    return (preambleSymbols + payloadSymbols) * symbolSeconds * 1000.0;
}

const lora::RadioSettings &Sx1276Radio::getSettings() const {
    return settings_;
}

bool Sx1276Radio::isInitialized() const {
    return initialized_;
}

void Sx1276Radio::dumpRegisters() {
    LOGI_LORA("Register dump (0x00..0x70)");
    for (uint16_t base = 0; base <= 0x70; base += 16) {
        char line[128];
        int used = std::snprintf(line, sizeof(line), "%02X: ", base);
        for (uint16_t i = 0; i < 16 && (base + i) <= 0x70; ++i) {
            uint8_t value = 0;
            readRegister(static_cast<uint8_t>(base + i), value);
            used += std::snprintf(line + used, sizeof(line) - used, "%02X ", value);
            if (used >= static_cast<int>(sizeof(line) - 4)) break;
        }
        LOGI_LORA("%s", line);
    }
}

const char *radioResultToString(RadioResult result) {
    switch (result) {
        case RadioResult::Ok: return "OK";
        case RadioResult::InvalidArgument: return "INVALID_ARGUMENT";
        case RadioResult::SpiError: return "SPI_ERROR";
        case RadioResult::VersionMismatch: return "VERSION_MISMATCH";
        case RadioResult::Timeout: return "TIMEOUT";
        case RadioResult::CrcError: return "CRC_ERROR";
        case RadioResult::NoPacket: return "NO_PACKET";
        case RadioResult::InvalidState: return "INVALID_STATE";
        case RadioResult::HardwareError: return "HARDWARE_ERROR";
        default: return "UNKNOWN";
    }
}

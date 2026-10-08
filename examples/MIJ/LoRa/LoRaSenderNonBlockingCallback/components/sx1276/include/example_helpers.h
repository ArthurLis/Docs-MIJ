#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "config.h"
#include "logger.h"
#include "lora_driver.h"
#include "lora_registers.h"

namespace example {

inline void delayMs(uint32_t ms) {
    TickType_t ticks = pdMS_TO_TICKS(ms);
    if (ticks == 0) {
        ticks = 1;
    }
    vTaskDelay(ticks);
}

inline uint64_t nowMs() {
    return static_cast<uint64_t>(esp_timer_get_time() / 1000LL);
}

inline lora::RadioSettings makeSettings() {
    lora::RadioSettings settings{};
    settings.frequencyHz = appcfg::DEFAULT_FREQUENCY_HZ;
    settings.bandwidthCode = appcfg::DEFAULT_BANDWIDTH_CODE;
    settings.spreadingFactor = appcfg::DEFAULT_SPREADING_FACTOR;
    settings.codingRate = appcfg::DEFAULT_CODING_RATE;
    settings.txPowerDbm = appcfg::DEFAULT_TX_POWER_DBM;
    settings.preambleSymbols = appcfg::DEFAULT_PREAMBLE_SYMBOLS;
    settings.crcEnabled = appcfg::DEFAULT_CRC_ENABLED;
    settings.implicitHeader = appcfg::DEFAULT_IMPLICIT_HEADER;
    settings.syncWord = appcfg::DEFAULT_SYNC_WORD;
    settings.iqInverted = appcfg::DEFAULT_IQ_INVERTED;
    settings.paBoost = appcfg::DEFAULT_PA_BOOST;
    settings.rxSymbolTimeout = appcfg::DEFAULT_RX_SYMBOL_TIMEOUT;
    settings.implicitPayloadLength = appcfg::DEFAULT_IMPLICIT_PAYLOAD_LENGTH;
    return settings;
}

inline bool initializeRadio(Sx1276Radio &radio, const char *exampleName) {
    LOGI_MAIN("============================================================");
    LOGI_MAIN("%s", exampleName);
    LOGI_MAIN("ESP-IDF 5.3.1 | ESP32 | SX1276 | LoRa raw");
    LOGI_MAIN("Frequency: %lu Hz", static_cast<unsigned long>(appcfg::DEFAULT_FREQUENCY_HZ));
    LOGI_MAIN("============================================================");

    const lora::RadioResult result = radio.begin(makeSettings());
    if (result != lora::RadioResult::Ok) {
        LOGE_MAIN("SX1276 init failed: %s", radioResultToString(result));
        return false;
    }

    LOGI_MAIN("SX1276 ready. RegVersion=0x%02X", radio.getVersion());
    return true;
}

[[noreturn]] inline void stopOnError() {
    while (true) {
        delayMs(1000);
    }
}

inline void logPacket(const uint8_t *data, const lora::PacketInfo &info) {
    char text[appcfg::RADIO_MAX_PAYLOAD + 1]{};
    const size_t copyLength = std::min(info.length, sizeof(text) - 1);
    if (copyLength > 0) {
        std::memcpy(text, data, copyLength);
    }
    text[copyLength] = '\0';

    LOGI_MAIN("Received %u bytes: \"%s\"", static_cast<unsigned>(info.length), text);
    LOGI_MAIN("RSSI=%.1f dBm | SNR=%.2f dB | FreqError=%.1f Hz",
              info.rssiDbm,
              info.snrDb,
              info.frequencyErrorHz);
}

inline lora::RadioResult applyTxIq(Sx1276Radio &radio) {
    uint8_t reg = 0;
    if (radio.readRegister(sx1276::REG_INVERT_IQ, reg) != lora::RadioResult::Ok) {
        return lora::RadioResult::SpiError;
    }

    reg &= sx1276::INVERT_IQ_TX_MASK;
    reg &= sx1276::INVERT_IQ_RX_MASK;
    reg |= sx1276::INVERT_IQ_RX_OFF;
    reg |= radio.getSettings().iqInverted ? sx1276::INVERT_IQ_TX_ON : sx1276::INVERT_IQ_TX_OFF;

    if (radio.writeRegister(sx1276::REG_INVERT_IQ, reg) != lora::RadioResult::Ok) {
        return lora::RadioResult::SpiError;
    }

    return radio.writeRegister(
        sx1276::REG_INVERT_IQ_2,
        radio.getSettings().iqInverted ? sx1276::INVERT_IQ_2_ON : sx1276::INVERT_IQ_2_OFF);
}

// Starts a real SX1276 asynchronous transmission. The call returns immediately
// after the radio enters TX mode; IRQ_TX_DONE is handled later by the application.
inline lora::RadioResult startTransmitAsync(Sx1276Radio &radio,
                                            const uint8_t *data,
                                            size_t length) {
    if (data == nullptr || length == 0 || length > appcfg::RADIO_MAX_PAYLOAD) {
        return lora::RadioResult::InvalidArgument;
    }

    lora::RadioResult result = radio.standby();
    if (result != lora::RadioResult::Ok) return result;

    radio.setTxContinuousMode(false);
    if ((result = applyTxIq(radio)) != lora::RadioResult::Ok) return result;

    uint8_t mapping = 0;
    if (radio.readRegister(sx1276::REG_DIO_MAPPING_1, mapping) != lora::RadioResult::Ok) {
        return lora::RadioResult::SpiError;
    }
    mapping = static_cast<uint8_t>((mapping & sx1276::DIO0_MASK) | sx1276::DIO0_TX_DONE);
    if (radio.writeRegister(sx1276::REG_DIO_MAPPING_1, mapping) != lora::RadioResult::Ok) {
        return lora::RadioResult::SpiError;
    }

    radio.writeRegister(sx1276::REG_IRQ_FLAGS, 0xFF);
    radio.writeRegister(sx1276::REG_FIFO_TX_BASE_ADDR, 0x00);
    radio.writeRegister(sx1276::REG_FIFO_ADDR_PTR, 0x00);
    radio.writeRegister(sx1276::REG_PAYLOAD_LENGTH, static_cast<uint8_t>(length));

    if (radio.writeBuffer(sx1276::REG_FIFO, data, length) != lora::RadioResult::Ok) {
        return lora::RadioResult::SpiError;
    }

    return radio.setMode(lora::RadioMode::Tx);
}

inline bool finishTransmitIfDone(Sx1276Radio &radio) {
    const uint8_t flags = radio.getIrqFlags();
    if ((flags & sx1276::IRQ_TX_DONE) == 0) {
        return false;
    }

    radio.writeRegister(sx1276::REG_IRQ_FLAGS, sx1276::IRQ_TX_DONE);
    radio.standby();
    return true;
}

inline lora::RadioResult startCadAsync(Sx1276Radio &radio) {
    lora::RadioResult result = radio.standby();
    if (result != lora::RadioResult::Ok) return result;

    radio.writeRegister(sx1276::REG_IRQ_FLAGS, 0xFF);

    uint8_t mapping = 0;
    if (radio.readRegister(sx1276::REG_DIO_MAPPING_1, mapping) != lora::RadioResult::Ok) {
        return lora::RadioResult::SpiError;
    }
    mapping = static_cast<uint8_t>((mapping & sx1276::DIO0_MASK) | sx1276::DIO0_CAD_DONE);
    if (radio.writeRegister(sx1276::REG_DIO_MAPPING_1, mapping) != lora::RadioResult::Ok) {
        return lora::RadioResult::SpiError;
    }

    return radio.setMode(lora::RadioMode::Cad);
}

inline bool readCadIfDone(Sx1276Radio &radio, bool &detected) {
    const uint8_t flags = radio.getIrqFlags();
    if ((flags & sx1276::IRQ_CAD_DONE) == 0) {
        return false;
    }

    detected = (flags & sx1276::IRQ_CAD_DETECTED) != 0;
    radio.writeRegister(
        sx1276::REG_IRQ_FLAGS,
        static_cast<uint8_t>(sx1276::IRQ_CAD_DONE | sx1276::IRQ_CAD_DETECTED));
    radio.standby();
    return true;
}

} // namespace example

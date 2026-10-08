#include "radio_common.h"

#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logger.h"

namespace radioapp {

Context::Context()
    : hal(appcfg::LORA_PIN_SCLK,
          appcfg::LORA_PIN_MISO,
          appcfg::LORA_PIN_MOSI,
          appcfg::LORA_SPI_HOST,
          appcfg::LORA_SPI_CLOCK_HZ),
      module(&hal,
             appcfg::LORA_PIN_NSS,
             appcfg::LORA_PIN_DIO1,
             appcfg::LORA_PIN_RST,
             appcfg::LORA_PIN_BUSY),
      radio(&module) {
}

bool initialize(Context &context) {
    context.radio.tcxoVoltage = appcfg::SX1262_TCXO_VOLTAGE;
    context.radio.useRegulatorLDO = appcfg::SX1262_USE_REGULATOR_LDO;

    ConfigLoRa_t cfg{};
    cfg.frequency = appcfg::LORA_FREQUENCY_MHZ;
    cfg.bandwidth = appcfg::LORA_BANDWIDTH_KHZ;
    cfg.spreadingFactor = appcfg::LORA_SPREADING_FACTOR;
    cfg.codingRate = appcfg::LORA_CODING_RATE;
    cfg.syncWord = appcfg::LORA_SYNC_WORD;
    cfg.power = appcfg::LORA_TX_POWER_DBM;
    cfg.preambleLength = appcfg::LORA_PREAMBLE_LENGTH;

    LOGI_LORA("Initializing SX1262...");
    int16_t state = context.radio.begin(cfg);
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORA("SX1262 initialization failed: %d", state);
        return false;
    }

    state = context.radio.setDio2AsRfSwitch(appcfg::SX1262_USE_DIO2_RF_SWITCH);
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORA("Unable to configure DIO2 RF switch: %d", state);
        return false;
    }

    LOGI_LORA("SX1262 ready: %.3f MHz, BW %.1f kHz, SF%u, CR 4/%u, SyncWord=0x%02X",
              static_cast<double>(appcfg::LORA_FREQUENCY_MHZ),
              static_cast<double>(appcfg::LORA_BANDWIDTH_KHZ),
              appcfg::LORA_SPREADING_FACTOR,
              appcfg::LORA_CODING_RATE,
              appcfg::LORA_SYNC_WORD);
    return true;
}

void waitMs(uint32_t milliseconds) {
    TickType_t ticks = pdMS_TO_TICKS(milliseconds);
    if (ticks == 0) ticks = 1;
    vTaskDelay(ticks);
}

void stopOnError() {
    LOGE_MAIN("Example stopped because initialization/configuration failed.");
    while (true) waitMs(1000);
}

void logPacket(SX1262 &radio, const uint8_t *data, size_t length) {
    char printable[appcfg::MAX_PACKET_BYTES + 1]{};
    const size_t copyLength = length < appcfg::MAX_PACKET_BYTES ? length : appcfg::MAX_PACKET_BYTES;
    for (size_t i = 0; i < copyLength; ++i) {
        const uint8_t value = data[i];
        printable[i] = (value >= 32 && value <= 126) ? static_cast<char>(value) : '.';
    }
    printable[copyLength] = '\0';

    LOGI_LORA("Packet received: %u bytes | payload='%s'",
              static_cast<unsigned>(length), printable);
    LOGI_LORA("RSSI=%.1f dBm | SNR=%.2f dB | FreqError=%.1f Hz",
              static_cast<double>(radio.getRSSI()),
              static_cast<double>(radio.getSNR()),
              static_cast<double>(radio.getFrequencyError()));
}

} // namespace radioapp

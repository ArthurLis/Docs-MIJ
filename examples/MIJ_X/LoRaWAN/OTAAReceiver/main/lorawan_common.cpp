#include "lorawan_common.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logger.h"
#include "lorawan_credentials.h"
#include "lorawan_storage.h"

namespace {

bool isActivationSuccess(int16_t state) {
    return state == RADIOLIB_ERR_NONE ||
           state == RADIOLIB_LORAWAN_SESSION_RESTORED ||
           state == RADIOLIB_LORAWAN_NEW_SESSION;
}

const char *activationStateToString(int16_t state) {
    switch (state) {
        case RADIOLIB_ERR_NONE: return "already active";
        case RADIOLIB_LORAWAN_SESSION_RESTORED: return "session restored";
        case RADIOLIB_LORAWAN_NEW_SESSION: return "new session";
        default: return "activation error";
    }
}

void logHexPayload(const uint8_t *data, size_t length) {
    if (!data || length == 0) {
        LOGI_LORAWAN("Downlink payload: <empty>");
        return;
    }
    char line[3 * 32 + 1]{};
    size_t offset = 0;
    for (size_t index = 0; index < length; ++index) {
        if (offset + 4 >= sizeof(line)) {
            LOGI_LORAWAN("Downlink HEX: %s", line);
            offset = 0;
            line[0] = '\0';
        }
        const int written = std::snprintf(line + offset, sizeof(line) - offset, "%02X%s",
                                          data[index], (index + 1 < length) ? " " : "");
        if (written > 0) offset += static_cast<size_t>(written);
    }
    if (offset > 0) LOGI_LORAWAN("Downlink HEX: %s", line);
}

} // namespace

namespace lorawan {

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
      radio(&module),
      node(&radio, appcfg::LORAWAN_BAND, appcfg::LORAWAN_SUB_BAND) {
}

bool initializeRadio(Context &context) {
    context.radio.tcxoVoltage = appcfg::SX1262_TCXO_VOLTAGE;
    context.radio.useRegulatorLDO = appcfg::SX1262_USE_REGULATOR_LDO;

    ConfigLoRa_t radioConfig{};
    radioConfig.frequency = appcfg::RADIO_INITIAL_FREQUENCY_MHZ;
    radioConfig.bandwidth = appcfg::RADIO_INITIAL_BANDWIDTH_KHZ;
    radioConfig.spreadingFactor = appcfg::RADIO_INITIAL_SPREADING_FACTOR;
    radioConfig.codingRate = appcfg::RADIO_INITIAL_CODING_RATE;
    radioConfig.syncWord = appcfg::LORAWAN_SYNC_WORD;
    radioConfig.power = appcfg::DEFAULT_TX_POWER_DBM;
    radioConfig.preambleLength = appcfg::RADIO_PREAMBLE_LENGTH;

    LOGI_LORAWAN("Initializing SX1262...");
    int16_t state = context.radio.begin(radioConfig);
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORAWAN("SX1262 initialization failed: %d", state);
        return false;
    }
    state = context.radio.setDio2AsRfSwitch(appcfg::SX1262_USE_DIO2_RF_SWITCH);
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORAWAN("Unable to configure DIO2 RF switch: %d", state);
        return false;
    }

    context.node.setADR(appcfg::ENABLE_ADR);
    context.node.setDutyCycle(true);
    LOGI_LORAWAN("SX1262 ready; LoRaWAN Class A, sub-band %u", appcfg::LORAWAN_SUB_BAND);
    return true;
}

bool configureUplinkDr(Context &context) {
    const int16_t state = context.node.setDatarate(appcfg::DEFAULT_UPLINK_DR);
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORAWAN("Unable to set uplink DR%u: %d", appcfg::DEFAULT_UPLINK_DR, state);
        return false;
    }
    LOGI_LORAWAN("Initial uplink DR set to DR%u; max application payload now %u bytes",
                 appcfg::DEFAULT_UPLINK_DR,
                 context.node.getMaxPayloadLen());
    return true;
}

#if defined(LORAWAN_EXAMPLE_ABP)
bool activateAbp(Context &context) {
    credentials::AbpCredentials values{};
    if (!credentials::loadAbp(values)) return false;

    const uint8_t *fNwkSIntKey = values.useLoRaWan11 ? values.fNwkSIntKey.data() : nullptr;
    const uint8_t *sNwkSIntKey = values.useLoRaWan11 ? values.sNwkSIntKey.data() : nullptr;
    const uint8_t *nwkSEncKey = values.useLoRaWan11 ? values.nwkSEncKey.data() : values.nwkSKey.data();

    int16_t state = context.node.beginABP(values.devAddr, fNwkSIntKey, sNwkSIntKey,
                                          nwkSEncKey, values.appSKey.data());
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORAWAN("ABP credential setup failed: %d", state);
        return false;
    }

    lorawanStorage::restore(context.node, appcfg::ABP_NVS_NAMESPACE);
    state = context.node.activateABP();
    if (!isActivationSuccess(state)) {
        LOGE_LORAWAN("ABP activation failed: %d", state);
        return false;
    }
    if (!configureUplinkDr(context)) return false;

    lorawanStorage::save(context.node, appcfg::ABP_NVS_NAMESPACE);
    LOGI_LORAWAN("ABP active (LoRaWAN %s, %s). Credentials are never printed.",
                 values.useLoRaWan11 ? "1.1" : "1.0.x", activationStateToString(state));
    return true;
}
#endif

#if defined(LORAWAN_EXAMPLE_OTAA)
bool activateOtaa(Context &context) {
    credentials::OtaaCredentials values{};
    if (!credentials::loadOtaa(values)) return false;

    const uint8_t *nwkKey = values.useLoRaWan11 ? values.nwkKey.data() : nullptr;
    int16_t state = context.node.beginOTAA(values.joinEui, values.devEui, nwkKey, values.appKey.data());
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORAWAN("OTAA credential setup failed: %d", state);
        return false;
    }

    lorawanStorage::restore(context.node, appcfg::OTAA_NVS_NAMESPACE);
    while (true) {
        LoRaWANJoinEvent_t joinEvent{};
        LOGI_LORAWAN("Activating OTAA...");
        state = context.node.activateOTAA(&joinEvent);
        lorawanStorage::save(context.node, appcfg::OTAA_NVS_NAMESPACE);

        if (isActivationSuccess(state)) {
            if (!configureUplinkDr(context)) return false;
            LOGI_LORAWAN("OTAA active (LoRaWAN %s, %s, newSession=%s).",
                         values.useLoRaWan11 ? "1.1" : "1.0.x",
                         activationStateToString(state), joinEvent.newSession ? "yes" : "no");
            return true;
        }

        LOGW_LORAWAN("OTAA activation failed: %d. Retrying in %lu ms.",
                     state, static_cast<unsigned long>(appcfg::OTAA_JOIN_RETRY_MS));
        waitMs(appcfg::OTAA_JOIN_RETRY_MS);
    }
}
#endif

void waitMs(uint32_t milliseconds) {
    TickType_t ticks = pdMS_TO_TICKS(milliseconds);
    if (ticks == 0) ticks = 1;
    vTaskDelay(ticks);
}

void waitForNextUplink(LoRaWANNode &node, uint32_t requestedDelayMs) {
    const RadioLibTime_t regulatoryDelay = node.timeUntilUplink();
    const uint32_t safeRegulatoryDelay = regulatoryDelay > 0 ? static_cast<uint32_t>(regulatoryDelay) : 0U;
    const uint32_t waitTime = std::max(requestedDelayMs, safeRegulatoryDelay);
    waitMs(waitTime);
}

void logDownlink(SX1262 &radio, int16_t window, const uint8_t *data, size_t length, const LoRaWANEvent_t &event) {
    LOGI_LORAWAN("Downlink received in RX%d: fPort=%u, fCnt=%lu, bytes=%u, RSSI=%.1f dBm, SNR=%.2f dB",
                 window, event.fPort, static_cast<unsigned long>(event.fCnt),
                 static_cast<unsigned>(length), static_cast<double>(radio.getRSSI()),
                 static_cast<double>(radio.getSNR()));
    logHexPayload(data, length);
}

size_t buildExamplePayload(char *buffer, size_t capacity, const char *prefix, uint32_t counter) {
    if (!buffer || capacity == 0) return 0;
    const size_t target = std::min(appcfg::EXAMPLE_PAYLOAD_SIZE, capacity - 1);
    const int written = std::snprintf(buffer, capacity, "%s #%08lu", prefix,
                                      static_cast<unsigned long>(counter));
    if (written < 0) return 0;
    size_t length = std::min(static_cast<size_t>(written), target);
    while (length < target) buffer[length++] = '.';
    buffer[length] = '\0';
    return length;
}

void stopOnError() {
    LOGE_MAIN("Example stopped because initialization/configuration failed.");
    while (true) waitMs(1000);
}

} // namespace lorawan

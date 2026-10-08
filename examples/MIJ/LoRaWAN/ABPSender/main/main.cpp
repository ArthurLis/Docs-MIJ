#include <cstdio>
#include <cstring>

#include "config.h"
#include "logger.h"
#include "lorawan_common.h"
#include "lorawan_storage.h"

extern "C" void app_main() {
    LOGI_MAIN("%s v%s", appcfg::FIRMWARE_NAME, appcfg::FIRMWARE_VERSION);

    LOGI_MAIN("Initializing NVS...");
    if (!lorawanStorage::initialize()) {
        lorawan::stopOnError();
    }
    LOGI_MAIN("NVS ready");

    LOGI_MAIN("Creating LoRaWAN radio context...");
    // RadioLib LoRaWAN objects are relatively large. Keep them out of the
    // ESP-IDF main task stack (default is small) to avoid stack corruption.
    static lorawan::Context context;
    LOGI_MAIN("LoRaWAN radio context ready");
    if (!lorawan::initializeRadio(context) || !lorawan::activateAbp(context)) {
        lorawan::stopOnError();
    }

    uint32_t counter = 0;
    while (true) {
        char payload[64]{};
        const int written = std::snprintf(payload, sizeof(payload), "ABP message #%lu",
                                          static_cast<unsigned long>(counter++));
        if (written <= 0) {
            LOGE_MAIN("Unable to create payload");
            lorawan::waitForNextUplink(context.node, appcfg::SEND_INTERVAL_MS);
            continue;
        }

        // Keep the maximum LoRaWAN downlink buffer out of the task stack too.
        static uint8_t downlink[appcfg::MAX_DOWNLINK_BYTES]{};
        size_t downlinkLength = sizeof(downlink);
        LoRaWANEvent_t uplinkEvent{};
        LoRaWANEvent_t downlinkEvent{};

        LOGI_LORAWAN("Sending ABP uplink: '%s'", payload);
        const int16_t state = context.node.sendReceive(
            reinterpret_cast<const uint8_t *>(payload),
            static_cast<size_t>(written),
            appcfg::UPLINK_FPORT,
            downlink,
            &downlinkLength,
            appcfg::UPLINK_CONFIRMED,
            &uplinkEvent,
            &downlinkEvent);

        lorawanStorage::save(context.node, appcfg::ABP_NVS_NAMESPACE);

        if (state < RADIOLIB_ERR_NONE) {
            LOGE_LORAWAN("Uplink failed: %d", state);
        } else {
            LOGI_LORAWAN("Uplink sent: fCnt=%lu, fPort=%u, DR=%u, freq=%.3f MHz, power=%d dBm",
                         static_cast<unsigned long>(uplinkEvent.fCnt), uplinkEvent.fPort,
                         uplinkEvent.datarate, static_cast<double>(uplinkEvent.freq), uplinkEvent.power);
            if (state > RADIOLIB_ERR_NONE) {
                lorawan::logDownlink(context.radio, state, downlink, downlinkLength, downlinkEvent);
            }
        }

        lorawan::waitForNextUplink(context.node, appcfg::SEND_INTERVAL_MS);
    }
}

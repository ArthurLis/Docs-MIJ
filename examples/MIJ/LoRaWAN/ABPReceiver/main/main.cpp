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

    LOGI_LORAWAN("Class A receiver ready.");
    LOGI_LORAWAN("Queue a downlink on the network server. This example sends an EMPTY uplink to open RX1/RX2.");

    while (true) {
        // Keep the maximum LoRaWAN downlink buffer out of the task stack too.
        static uint8_t downlink[appcfg::MAX_DOWNLINK_BYTES]{};
        size_t downlinkLength = sizeof(downlink);
        LoRaWANEvent_t uplinkEvent{};
        LoRaWANEvent_t downlinkEvent{};

        LOGI_LORAWAN("Sending empty uplink to open RX1/RX2...");
        const int16_t state = context.node.sendReceive(
            nullptr,
            0,
            appcfg::UPLINK_FPORT,
            downlink,
            &downlinkLength,
            false,
            &uplinkEvent,
            &downlinkEvent);

        lorawanStorage::save(context.node, appcfg::ABP_NVS_NAMESPACE);

        if (state < RADIOLIB_ERR_NONE) {
            LOGE_LORAWAN("Empty uplink/RX windows failed: %d", state);
        } else if (state == RADIOLIB_ERR_NONE) {
            LOGI_LORAWAN("No downlink in RX1/RX2 (uplink fCnt=%lu)",
                         static_cast<unsigned long>(uplinkEvent.fCnt));
        } else {
            lorawan::logDownlink(context.radio, state, downlink, downlinkLength, downlinkEvent);
        }

        lorawan::waitForNextUplink(context.node, appcfg::DOWNLINK_POLL_INTERVAL_MS);
    }
}

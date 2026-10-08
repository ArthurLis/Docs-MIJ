#include <cstddef>
#include <cstdint>

#include "config.h"
#include "logger.h"
#include "lorawan_common.h"
#include "lorawan_storage.h"

extern "C" void app_main() {
    LOGI_MAIN("%s v%s", appcfg::FIRMWARE_NAME, appcfg::FIRMWARE_VERSION);
    LOGI_MAIN("Initializing NVS...");
    if (!lorawanStorage::initialize()) lorawan::stopOnError();
    LOGI_MAIN("NVS ready");

    static lorawan::Context context;
    if (!lorawan::initializeRadio(context) || !lorawan::activateAbp(context)) lorawan::stopOnError();

    static uint8_t downlink[appcfg::MAX_DOWNLINK_BYTES]{};
    while (true) {
        size_t downlinkLength = sizeof(downlink);
        LoRaWANEvent_t uplinkEvent{};
        LoRaWANEvent_t downlinkEvent{};

        LOGI_LORAWAN("Sending empty ABP uplink to open RX1/RX2...");
        const int16_t state = context.node.sendReceive(
            nullptr, 0, appcfg::UPLINK_FPORT,
            downlink, &downlinkLength, false,
            &uplinkEvent, &downlinkEvent);

        lorawanStorage::save(context.node, appcfg::ABP_NVS_NAMESPACE);
        if (state < RADIOLIB_ERR_NONE) {
            LOGE_LORAWAN("Empty uplink/downlink cycle failed: %d", state);
        } else if (state > RADIOLIB_ERR_NONE) {
            lorawan::logDownlink(context.radio, state, downlink, downlinkLength, downlinkEvent);
        } else {
            LOGI_LORAWAN("No downlink in RX1/RX2");
        }
        lorawan::waitForNextUplink(context.node, appcfg::DOWNLINK_POLL_INTERVAL_MS);
    }
}

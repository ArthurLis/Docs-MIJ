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

    uint32_t counter = 0;
    static uint8_t downlink[appcfg::MAX_DOWNLINK_BYTES]{};
    char payload[appcfg::EXAMPLE_PAYLOAD_SIZE + 32]{};

    while (true) {
        const size_t payloadLength = lorawan::buildExamplePayload(
            payload, sizeof(payload), "ABP", counter++);
        const uint8_t maxPayload = context.node.getMaxPayloadLen();
        if (payloadLength == 0 || payloadLength > maxPayload) {
            LOGE_LORAWAN("Requested payload=%u bytes exceeds current maximum=%u bytes at DR%u",
                         static_cast<unsigned>(payloadLength), maxPayload, appcfg::DEFAULT_UPLINK_DR);
            lorawan::waitForNextUplink(context.node, appcfg::SEND_INTERVAL_MS);
            continue;
        }

        size_t downlinkLength = sizeof(downlink);
        LoRaWANEvent_t uplinkEvent{};
        LoRaWANEvent_t downlinkEvent{};
        LOGI_LORAWAN("Sending ABP uplink: %u bytes | '%s'",
                     static_cast<unsigned>(payloadLength), payload);

        const int16_t state = context.node.sendReceive(
            reinterpret_cast<const uint8_t *>(payload), payloadLength,
            appcfg::UPLINK_FPORT, downlink, &downlinkLength,
            appcfg::UPLINK_CONFIRMED, &uplinkEvent, &downlinkEvent);

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

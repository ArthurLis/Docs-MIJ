#include <cstddef>
#include <cstdint>

#include "config.h"
#include "logger.h"
#include "radio_common.h"

extern "C" void app_main() {
    LOGI_MAIN("%s v%s", appcfg::FIRMWARE_NAME, appcfg::FIRMWARE_VERSION);
    static radioapp::Context context;
    if (!radioapp::initialize(context)) radioapp::stopOnError();

    static uint8_t buffer[appcfg::MAX_PACKET_BYTES]{};
    while (true) {
        LOGD_LORA("Waiting for packet (blocking receive)...");
        const int16_t state = context.radio.receive(buffer, 0);
        if (state == RADIOLIB_ERR_NONE) {
            const size_t length = context.radio.getPacketLength();
            radioapp::logPacket(context.radio, buffer, length);
        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            LOGW_LORA("Packet discarded because CRC failed");
        } else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
            LOGE_LORA("Receive failed: %d", state);
        }
        radioapp::waitMs(1);
    }
}

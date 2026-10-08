#include <cstddef>
#include <cstdint>
#include "esp_attr.h"

#include "config.h"
#include "logger.h"
#include "radio_common.h"

namespace {
volatile bool packetReceived = false;

void IRAM_ATTR onPacketReceived() {
    packetReceived = true;
}
} // namespace

extern "C" void app_main() {
    LOGI_MAIN("%s v%s", appcfg::FIRMWARE_NAME, appcfg::FIRMWARE_VERSION);
    static radioapp::Context context;
    if (!radioapp::initialize(context)) radioapp::stopOnError();

    context.radio.setPacketReceivedAction(onPacketReceived);
    int16_t state = context.radio.startReceive();
    if (state != RADIOLIB_ERR_NONE) {
        LOGE_LORA("Unable to start continuous receive: %d", state);
        radioapp::stopOnError();
    }
    LOGI_LORA("Continuous receive active; DIO1 callback enabled");

    static uint8_t buffer[appcfg::MAX_PACKET_BYTES]{};
    while (true) {
        if (!packetReceived) {
            radioapp::waitMs(10);
            continue;
        }

        packetReceived = false;
        const size_t length = context.radio.getPacketLength();
        state = context.radio.readData(buffer, length);
        if (state == RADIOLIB_ERR_NONE) {
            radioapp::logPacket(context.radio, buffer, length);
        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            LOGW_LORA("Packet discarded because CRC failed");
        } else {
            LOGE_LORA("readData failed: %d", state);
        }
    }
}

#include <cstdint>

#include "example_helpers.h"

extern "C" void app_main() {
    Sx1276Radio radio;
    if (!example::initializeRadio(radio, "LoRaReceiver - polling receiver")) {
        example::stopOnError();
    }

    uint8_t buffer[appcfg::RADIO_MAX_PAYLOAD]{};
    lora::PacketInfo info{};

    while (true) {
        const lora::RadioResult result = radio.receive(buffer, sizeof(buffer), info, 1000);
        if (result == lora::RadioResult::Ok) {
            example::logPacket(buffer, info);
        } else if (result == lora::RadioResult::CrcError) {
            LOGW_MAIN("Packet discarded because CRC failed");
        } else if (result != lora::RadioResult::Timeout) {
            LOGE_MAIN("Receive error: %s", radioResultToString(result));
        }
    }
}

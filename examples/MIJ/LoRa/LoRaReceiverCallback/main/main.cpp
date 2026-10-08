#include <cstdint>

#include "example_helpers.h"

namespace {
void onReceive(const uint8_t *buffer, const lora::PacketInfo &info) {
    LOGI_MAIN("RX callback");
    example::logPacket(buffer, info);
}
}

extern "C" void app_main() {
    Sx1276Radio radio;
    if (!example::initializeRadio(radio, "LoRaReceiverCallback - DIO0 RxDone")) {
        example::stopOnError();
    }

    if (radio.startReceiveContinuous() != lora::RadioResult::Ok) {
        LOGE_MAIN("Could not enter continuous receive mode");
        example::stopOnError();
    }

    uint32_t lastDio0Count = radio.getDio0InterruptCount();
    uint8_t buffer[appcfg::RADIO_MAX_PAYLOAD]{};

    while (true) {
        const uint32_t currentCount = radio.getDio0InterruptCount();
        if (currentCount != lastDio0Count) {
            lastDio0Count = currentCount;
            lora::PacketInfo info{};
            const lora::RadioResult result = radio.pollReceivedPacket(buffer, sizeof(buffer), info);
            if (result == lora::RadioResult::Ok) {
                onReceive(buffer, info);
            } else if (result == lora::RadioResult::CrcError) {
                LOGW_MAIN("RX callback: CRC error");
            }
        }
        example::delayMs(1);
    }
}

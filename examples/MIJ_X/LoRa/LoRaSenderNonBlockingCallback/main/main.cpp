#include <cstdio>
#include <cstring>
#include "esp_attr.h"

#include "config.h"
#include "logger.h"
#include "radio_common.h"

namespace {
volatile bool transmissionFinished = false;

void IRAM_ATTR onPacketSent() {
    transmissionFinished = true;
}
} // namespace

extern "C" void app_main() {
    LOGI_MAIN("%s v%s", appcfg::FIRMWARE_NAME, appcfg::FIRMWARE_VERSION);
    static radioapp::Context context;
    if (!radioapp::initialize(context)) radioapp::stopOnError();

    context.radio.setPacketSentAction(onPacketSent);

    uint32_t packetCounter = 0;
    uint32_t workCounter = 0;
    char payload[64]{};

    while (true) {
        const int length = std::snprintf(payload, sizeof(payload), "JVTECH async LoRa #%lu",
                                         static_cast<unsigned long>(packetCounter++));
        if (length <= 0) {
            LOGE_MAIN("Unable to create payload");
            radioapp::waitMs(appcfg::SEND_INTERVAL_MS);
            continue;
        }

        transmissionFinished = false;
        const int16_t startState = context.radio.startTransmit(
            reinterpret_cast<const uint8_t *>(payload), static_cast<size_t>(length));
        if (startState != RADIOLIB_ERR_NONE) {
            LOGE_LORA("startTransmit failed: %d", startState);
            radioapp::waitMs(appcfg::SEND_INTERVAL_MS);
            continue;
        }

        LOGI_LORA("Non-blocking TX started: '%s'", payload);
        while (!transmissionFinished) {
            LOGD_MAIN("Main task is still running while SX1262 transmits: %lu",
                      static_cast<unsigned long>(workCounter++));
            radioapp::waitMs(appcfg::MAIN_WORK_INTERVAL_MS);
        }

        const int16_t finishState = context.radio.finishTransmit();
        if (finishState == RADIOLIB_ERR_NONE) {
            LOGI_LORA("TX-done callback received; transmission finalized");
        } else {
            LOGE_LORA("finishTransmit failed: %d", finishState);
        }
        radioapp::waitMs(appcfg::SEND_INTERVAL_MS);
    }
}

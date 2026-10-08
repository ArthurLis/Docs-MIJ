#include <cstdio>
#include <cstring>

#include "config.h"
#include "logger.h"
#include "radio_common.h"

extern "C" void app_main() {
    LOGI_MAIN("%s v%s", appcfg::FIRMWARE_NAME, appcfg::FIRMWARE_VERSION);
    static radioapp::Context context;
    if (!radioapp::initialize(context)) radioapp::stopOnError();

    uint32_t counter = 0;
    while (true) {
        char payload[64]{};
        const int length = std::snprintf(payload, sizeof(payload), "JVTECH LoRa packet #%lu",
                                         static_cast<unsigned long>(counter++));
        if (length <= 0) {
            LOGE_MAIN("Unable to create payload");
            radioapp::waitMs(appcfg::SEND_INTERVAL_MS);
            continue;
        }

        LOGI_LORA("Sending %d bytes: '%s'", length, payload);
        const int16_t state = context.radio.transmit(
            reinterpret_cast<const uint8_t *>(payload), static_cast<size_t>(length));

        if (state == RADIOLIB_ERR_NONE) {
            LOGI_LORA("Transmission completed");
        } else {
            LOGE_LORA("Transmission failed: %d", state);
        }
        radioapp::waitMs(appcfg::SEND_INTERVAL_MS);
    }
}

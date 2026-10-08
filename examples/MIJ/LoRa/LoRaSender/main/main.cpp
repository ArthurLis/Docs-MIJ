#include <cstdio>
#include <cstring>

#include "example_helpers.h"

extern "C" void app_main() {
    Sx1276Radio radio;
    if (!example::initializeRadio(radio, "LoRaSender - blocking transmitter")) {
        example::stopOnError();
    }

    uint32_t counter = 0;
    while (true) {
        char message[64];
        const int length = std::snprintf(message, sizeof(message), "hello %lu",
                                         static_cast<unsigned long>(counter));

        LOGI_MAIN("Sending packet: %lu", static_cast<unsigned long>(counter));
        const lora::RadioResult result = radio.transmit(
            reinterpret_cast<const uint8_t *>(message),
            static_cast<size_t>(length),
            appcfg::RADIO_TX_TIMEOUT_MS);

        if (result == lora::RadioResult::Ok) {
            LOGI_MAIN("TxDone");
        } else {
            LOGE_MAIN("Transmit failed: %s", radioResultToString(result));
        }

        counter++;
        example::delayMs(5000);
    }
}

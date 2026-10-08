#include <cstdio>

#include "example_helpers.h"

namespace {
void onTxDone() {
    LOGI_MAIN("TxDone callback");
}
}

extern "C" void app_main() {
    Sx1276Radio radio;
    if (!example::initializeRadio(radio, "LoRaSenderNonBlockingCallback - DIO0 TxDone")) {
        example::stopOnError();
    }

    uint32_t counter = 0;
    bool txBusy = false;
    uint64_t nextSendMs = 0;
    uint32_t lastDio0Count = radio.getDio0InterruptCount();

    while (true) {
        const uint32_t currentCount = radio.getDio0InterruptCount();
        if (txBusy && currentCount != lastDio0Count) {
            lastDio0Count = currentCount;
            if (example::finishTransmitIfDone(radio)) {
                txBusy = false;
                onTxDone();
            }
        }

        const uint64_t now = example::nowMs();
        if (!txBusy && now >= nextSendMs) {
            char message[64];
            const int length = std::snprintf(message, sizeof(message), "hello %lu",
                                             static_cast<unsigned long>(counter));
            LOGI_MAIN("Sending packet non-blocking: %lu", static_cast<unsigned long>(counter));

            const lora::RadioResult result = example::startTransmitAsync(
                radio,
                reinterpret_cast<const uint8_t *>(message),
                static_cast<size_t>(length));
            if (result == lora::RadioResult::Ok) {
                txBusy = true;
                lastDio0Count = radio.getDio0InterruptCount();
                counter++;
                nextSendMs = now + 5000;
            } else {
                LOGE_MAIN("Async TX start failed: %s", radioResultToString(result));
                nextSendMs = now + 1000;
            }
        }

        example::delayMs(1);
    }
}

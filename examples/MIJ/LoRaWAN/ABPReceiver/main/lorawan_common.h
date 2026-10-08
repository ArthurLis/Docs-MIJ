#pragma once

#include <cstddef>
#include <cstdint>

#include "RadioLib.h"
#include "config.h"

namespace lorawan {

struct Context {
    EspHal hal;
    Module module;
    SX1276 radio;
    LoRaWANNode node;

    Context();
};

bool initializeRadio(Context &context);
#if defined(LORAWAN_EXAMPLE_ABP)
bool activateAbp(Context &context);
#endif
#if defined(LORAWAN_EXAMPLE_OTAA)
bool activateOtaa(Context &context);
#endif
void waitMs(uint32_t milliseconds);
void waitForNextUplink(LoRaWANNode &node, uint32_t requestedDelayMs);
void logDownlink(SX1276 &radio, int16_t window, const uint8_t *data, size_t length, const LoRaWANEvent_t &event);
void stopOnError();

} // namespace lorawan

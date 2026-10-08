#pragma once

#include <cstddef>
#include <cstdint>
#include "RadioLib.h"
#include "config.h"

namespace lorawan {

struct Context {
    EspHal hal;
    Module module;
    SX1262 radio;
    LoRaWANNode node;
    Context();
};

bool initializeRadio(Context &context);
bool configureUplinkDr(Context &context);
#if defined(LORAWAN_EXAMPLE_ABP)
bool activateAbp(Context &context);
#endif
#if defined(LORAWAN_EXAMPLE_OTAA)
bool activateOtaa(Context &context);
#endif
void waitMs(uint32_t milliseconds);
void waitForNextUplink(LoRaWANNode &node, uint32_t requestedDelayMs);
void logDownlink(SX1262 &radio, int16_t window, const uint8_t *data, size_t length, const LoRaWANEvent_t &event);
size_t buildExamplePayload(char *buffer, size_t capacity, const char *prefix, uint32_t counter);
void stopOnError();

} // namespace lorawan

#pragma once

#include <cstddef>
#include <cstdint>
#include "RadioLib.h"
#include "config.h"

namespace radioapp {

struct Context {
    EspHal hal;
    Module module;
    SX1262 radio;

    Context();
};

bool initialize(Context &context);
void waitMs(uint32_t milliseconds);
void stopOnError();
void logPacket(SX1262 &radio, const uint8_t *data, size_t length);

} // namespace radioapp

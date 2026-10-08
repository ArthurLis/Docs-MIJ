#pragma once

#include <array>
#include <cstdint>
#include "config.h"

namespace credentials {

#if defined(LORAWAN_EXAMPLE_ABP)
struct AbpCredentials {
    uint32_t devAddr = 0;
    std::array<uint8_t, 16> nwkSKey{};
    std::array<uint8_t, 16> appSKey{};
    std::array<uint8_t, 16> fNwkSIntKey{};
    std::array<uint8_t, 16> sNwkSIntKey{};
    std::array<uint8_t, 16> nwkSEncKey{};
    bool useLoRaWan11 = false;
};
bool loadAbp(AbpCredentials &out);
#endif

#if defined(LORAWAN_EXAMPLE_OTAA)
struct OtaaCredentials {
    uint64_t joinEui = 0;
    uint64_t devEui = 0;
    std::array<uint8_t, 16> appKey{};
    std::array<uint8_t, 16> nwkKey{};
    bool useLoRaWan11 = false;
};
bool loadOtaa(OtaaCredentials &out);
#endif

} // namespace credentials

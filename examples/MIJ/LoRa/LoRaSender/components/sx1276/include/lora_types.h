#pragma once

#include <cstddef>
#include <cstdint>

namespace lora {

enum class RadioMode : uint8_t {
    Sleep = 0,
    Standby = 1,
    Fstx = 2,
    Tx = 3,
    Fsrx = 4,
    RxContinuous = 5,
    RxSingle = 6,
    Cad = 7,
};

enum class RadioResult : uint8_t {
    Ok = 0,
    InvalidArgument,
    SpiError,
    VersionMismatch,
    Timeout,
    CrcError,
    NoPacket,
    InvalidState,
    HardwareError,
};

struct RadioSettings {
    uint32_t frequencyHz;
    uint8_t bandwidthCode;   // SX1276 register code: 0..9
    uint8_t spreadingFactor; // 6..12
    uint8_t codingRate;      // 1..4 => 4/5..4/8
    int8_t txPowerDbm;
    uint16_t preambleSymbols;
    bool crcEnabled;
    bool implicitHeader;
    uint8_t syncWord;
    bool iqInverted;
    bool paBoost;
    uint16_t rxSymbolTimeout;
    uint8_t implicitPayloadLength;
};

struct PacketInfo {
    size_t length;
    float rssiDbm;
    float snrDb;
    float frequencyErrorHz;
    uint8_t irqFlags;
};

struct CadResult {
    bool completed;
    bool detected;
    uint8_t irqFlags;
};

constexpr uint32_t bandwidthToHz(uint8_t code) {
    switch (code) {
        case 0: return 7'800;
        case 1: return 10'400;
        case 2: return 15'600;
        case 3: return 20'800;
        case 4: return 31'250;
        case 5: return 41'700;
        case 6: return 62'500;
        case 7: return 125'000;
        case 8: return 250'000;
        case 9: return 500'000;
        default: return 0;
    }
}

constexpr uint8_t codingRateDenominator(uint8_t code) {
    return static_cast<uint8_t>(code + 4);
}

inline RadioSettings defaultSettings() {
    return RadioSettings{
        915'000'000,
        7,
        7,
        1,
        17,
        8,
        true,
        false,
        0x12,
        false,
        true,
        100,
        32,
    };
}

} // namespace lora

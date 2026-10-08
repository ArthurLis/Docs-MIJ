#include "lorawan_credentials.h"

#include <cctype>
#include <cstddef>

#include "config.h"
#include "logger.h"

namespace {

int hexNibble(char value) {
    if (value >= '0' && value <= '9') return value - '0';
    value = static_cast<char>(std::toupper(static_cast<unsigned char>(value)));
    if (value >= 'A' && value <= 'F') return 10 + (value - 'A');
    return -1;
}

bool isSeparator(char value) {
    return value == ' ' || value == ':' || value == '-' || value == '_' ||
           value == ',' || value == '{' || value == '}';
}

bool normalizeHex(const char *source, char *destination, size_t destinationSize, size_t expectedDigits) {
    if (source == nullptr || destination == nullptr || destinationSize <= expectedDigits) return false;

    size_t sourceIndex = 0;
    size_t destinationIndex = 0;
    while (source[sourceIndex] != '\0') {
        const char current = source[sourceIndex++];
        if (isSeparator(current)) continue;

        if (current == '0' && (source[sourceIndex] == 'x' || source[sourceIndex] == 'X')) {
            sourceIndex++;
            continue;
        }

        if (hexNibble(current) < 0 || destinationIndex >= expectedDigits) return false;
        destination[destinationIndex++] = current;
    }

    destination[destinationIndex] = '\0';
    return destinationIndex == expectedDigits;
}

bool parseBytes16(const char *source, std::array<uint8_t, 16> &out) {
    char normalized[33]{};
    if (!normalizeHex(source, normalized, sizeof(normalized), 32)) return false;

    for (size_t index = 0; index < out.size(); ++index) {
        const int high = hexNibble(normalized[index * 2]);
        const int low = hexNibble(normalized[index * 2 + 1]);
        if (high < 0 || low < 0) return false;
        out[index] = static_cast<uint8_t>((high << 4) | low);
    }
    return true;
}

bool parseU64(const char *source, uint64_t &out) {
    char normalized[17]{};
    if (!normalizeHex(source, normalized, sizeof(normalized), 16)) return false;

    uint64_t value = 0;
    for (size_t index = 0; index < 16; ++index) {
        value = (value << 4) | static_cast<uint64_t>(hexNibble(normalized[index]));
    }
    out = value;
    return true;
}

bool parseU32(const char *source, uint32_t &out) {
    char normalized[9]{};
    if (!normalizeHex(source, normalized, sizeof(normalized), 8)) return false;

    uint32_t value = 0;
    for (size_t index = 0; index < 8; ++index) {
        value = (value << 4) | static_cast<uint32_t>(hexNibble(normalized[index]));
    }
    out = value;
    return true;
}

bool isEmpty(const char *value) {
    return value == nullptr || value[0] == '\0';
}

bool isAllZero(const std::array<uint8_t, 16> &value) {
    for (const uint8_t byte : value) {
        if (byte != 0) return false;
    }
    return true;
}

} // namespace

namespace credentials {

#if defined(LORAWAN_EXAMPLE_ABP)
bool loadAbp(AbpCredentials &out) {
    if (!parseU32(appcfg::ABP_DEV_ADDR_HEX, out.devAddr) || out.devAddr == 0) {
        LOGE_LORAWAN("Invalid ABP_DEV_ADDR_HEX. Paste the 8 hexadecimal digits from the network server.");
        return false;
    }
    if (!parseBytes16(appcfg::ABP_APP_S_KEY_HEX, out.appSKey) || isAllZero(out.appSKey)) {
        LOGE_LORAWAN("Invalid ABP_APP_S_KEY_HEX. Paste the 32 hexadecimal digits from the network server.");
        return false;
    }

    const bool any11 = !isEmpty(appcfg::ABP_F_NWK_S_INT_KEY_HEX) ||
                       !isEmpty(appcfg::ABP_S_NWK_S_INT_KEY_HEX) ||
                       !isEmpty(appcfg::ABP_NWK_S_ENC_KEY_HEX);
    if (any11) {
        if (!parseBytes16(appcfg::ABP_F_NWK_S_INT_KEY_HEX, out.fNwkSIntKey) || isAllZero(out.fNwkSIntKey) ||
            !parseBytes16(appcfg::ABP_S_NWK_S_INT_KEY_HEX, out.sNwkSIntKey) || isAllZero(out.sNwkSIntKey) ||
            !parseBytes16(appcfg::ABP_NWK_S_ENC_KEY_HEX, out.nwkSEncKey) || isAllZero(out.nwkSEncKey)) {
            LOGE_LORAWAN("LoRaWAN 1.1 ABP requires all three valid network session keys.");
            return false;
        }
        out.useLoRaWan11 = true;
    } else {
        if (!parseBytes16(appcfg::ABP_NWK_S_KEY_HEX, out.nwkSKey) || isAllZero(out.nwkSKey)) {
            LOGE_LORAWAN("Invalid ABP_NWK_S_KEY_HEX. Paste the 32 hexadecimal digits from the network server.");
            return false;
        }
    }
    return true;
}
#endif

#if defined(LORAWAN_EXAMPLE_OTAA)
bool loadOtaa(OtaaCredentials &out) {
    if (!parseU64(appcfg::OTAA_JOIN_EUI_HEX, out.joinEui)) {
        LOGE_LORAWAN("Invalid OTAA_JOIN_EUI_HEX. Expected 16 hexadecimal digits.");
        return false;
    }
    if (!parseU64(appcfg::OTAA_DEV_EUI_HEX, out.devEui) || out.devEui == 0) {
        LOGE_LORAWAN("Invalid OTAA_DEV_EUI_HEX. Paste the 16 hexadecimal digits from the network server.");
        return false;
    }
    if (!parseBytes16(appcfg::OTAA_APP_KEY_HEX, out.appKey) || isAllZero(out.appKey)) {
        LOGE_LORAWAN("Invalid OTAA_APP_KEY_HEX. Paste the 32 hexadecimal digits from the network server.");
        return false;
    }

    if (!isEmpty(appcfg::OTAA_NWK_KEY_HEX)) {
        if (!parseBytes16(appcfg::OTAA_NWK_KEY_HEX, out.nwkKey) || isAllZero(out.nwkKey)) {
            LOGE_LORAWAN("Invalid OTAA_NWK_KEY_HEX. Leave empty for 1.0.x or paste 32 hexadecimal digits for 1.1.");
            return false;
        }
        out.useLoRaWan11 = true;
    }
    return true;
}
#endif

} // namespace credentials

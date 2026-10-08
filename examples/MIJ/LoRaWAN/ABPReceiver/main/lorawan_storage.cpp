#include "lorawan_storage.h"

#include <array>
#include <cstring>

#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "logger.h"

namespace {

constexpr const char *NONCES_KEY = "nonces";
constexpr const char *SESSION_KEY = "session";

template <size_t Size>
bool readBlob(nvs_handle_t handle, const char *key, std::array<uint8_t, Size> &buffer) {
    size_t storedSize = buffer.size();
    const esp_err_t result = nvs_get_blob(handle, key, buffer.data(), &storedSize);
    return result == ESP_OK && storedSize == buffer.size();
}

template <size_t Size>
bool writeBlob(nvs_handle_t handle, const char *key, const uint8_t *data) {
    return nvs_set_blob(handle, key, data, Size) == ESP_OK;
}

} // namespace

namespace lorawanStorage {

bool initialize() {
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        result = nvs_flash_init();
    }
    if (result != ESP_OK) {
        LOGE_STORAGE("NVS initialization failed: %s", esp_err_to_name(result));
        return false;
    }
    return true;
}

bool restore(LoRaWANNode &node, const char *nameSpace) {
    nvs_handle_t handle = 0;
    if (nvs_open(nameSpace, NVS_READONLY, &handle) != ESP_OK) {
        LOGI_STORAGE("No persisted LoRaWAN state found");
        return false;
    }

    std::array<uint8_t, RADIOLIB_LORAWAN_NONCES_BUF_SIZE> nonces{};
    std::array<uint8_t, RADIOLIB_LORAWAN_SESSION_BUF_SIZE> session{};
    const bool hasNonces = readBlob(handle, NONCES_KEY, nonces);
    const bool hasSession = readBlob(handle, SESSION_KEY, session);
    nvs_close(handle);

    if (!hasNonces) {
        LOGI_STORAGE("No valid LoRaWAN nonce state found");
        return false;
    }

    int16_t state = node.setBufferNonces(nonces.data());
    if (state != RADIOLIB_ERR_NONE) {
        LOGW_STORAGE("Stored LoRaWAN nonce state rejected: %d", state);
        clear(nameSpace);
        return false;
    }

    if (hasSession) {
        state = node.setBufferSession(session.data());
        if (state != RADIOLIB_ERR_NONE) {
            LOGW_STORAGE("Stored LoRaWAN session rejected: %d", state);
            clear(nameSpace);
            return false;
        }
        LOGI_STORAGE("LoRaWAN session restored from NVS");
        return true;
    }

    LOGI_STORAGE("LoRaWAN nonces restored; no active session stored");
    return false;
}

bool save(LoRaWANNode &node, const char *nameSpace) {
    nvs_handle_t handle = 0;
    esp_err_t result = nvs_open(nameSpace, NVS_READWRITE, &handle);
    if (result != ESP_OK) {
        LOGE_STORAGE("Unable to open NVS namespace '%s': %s", nameSpace, esp_err_to_name(result));
        return false;
    }

    const bool noncesSaved = writeBlob<RADIOLIB_LORAWAN_NONCES_BUF_SIZE>(handle, NONCES_KEY, node.getBufferNonces());

    bool sessionSaved = true;
    if (node.isActivated()) {
        sessionSaved = writeBlob<RADIOLIB_LORAWAN_SESSION_BUF_SIZE>(handle, SESSION_KEY, node.getBufferSession());
    } else {
        const esp_err_t eraseResult = nvs_erase_key(handle, SESSION_KEY);
        sessionSaved = (eraseResult == ESP_OK || eraseResult == ESP_ERR_NVS_NOT_FOUND);
    }

    result = nvs_commit(handle);
    nvs_close(handle);

    if (!noncesSaved || !sessionSaved || result != ESP_OK) {
        LOGE_STORAGE("Failed to persist LoRaWAN state");
        return false;
    }
    return true;
}

bool clear(const char *nameSpace) {
    nvs_handle_t handle = 0;
    const esp_err_t openResult = nvs_open(nameSpace, NVS_READWRITE, &handle);
    if (openResult != ESP_OK) {
        return false;
    }
    const esp_err_t eraseResult = nvs_erase_all(handle);
    const esp_err_t commitResult = nvs_commit(handle);
    nvs_close(handle);
    return eraseResult == ESP_OK && commitResult == ESP_OK;
}

} // namespace lorawanStorage

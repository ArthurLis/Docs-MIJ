#pragma once

#include "esp_log.h"

#ifdef DBG_MAIN
#define LOGI_MAIN(...) ESP_LOGI("MAIN", __VA_ARGS__)
#define LOGW_MAIN(...) ESP_LOGW("MAIN", __VA_ARGS__)
#define LOGE_MAIN(...) ESP_LOGE("MAIN", __VA_ARGS__)
#define LOGD_MAIN(...) ESP_LOGD("MAIN", __VA_ARGS__)
#else
#define LOGI_MAIN(...)
#define LOGW_MAIN(...)
#define LOGE_MAIN(...)
#define LOGD_MAIN(...)
#endif

#ifdef DBG_LORA
#define LOGI_LORA(...) ESP_LOGI("LORA", __VA_ARGS__)
#define LOGW_LORA(...) ESP_LOGW("LORA", __VA_ARGS__)
#define LOGE_LORA(...) ESP_LOGE("LORA", __VA_ARGS__)
#define LOGD_LORA(...) ESP_LOGD("LORA", __VA_ARGS__)
#else
#define LOGI_LORA(...)
#define LOGW_LORA(...)
#define LOGE_LORA(...)
#define LOGD_LORA(...)
#endif

#ifdef DBG_LORAWAN
#define LOGI_LORAWAN(...) ESP_LOGI("LORAWAN", __VA_ARGS__)
#define LOGW_LORAWAN(...) ESP_LOGW("LORAWAN", __VA_ARGS__)
#define LOGE_LORAWAN(...) ESP_LOGE("LORAWAN", __VA_ARGS__)
#define LOGD_LORAWAN(...) ESP_LOGD("LORAWAN", __VA_ARGS__)
#else
#define LOGI_LORAWAN(...)
#define LOGW_LORAWAN(...)
#define LOGE_LORAWAN(...)
#define LOGD_LORAWAN(...)
#endif

#ifdef DBG_STORAGE
#define LOGI_STORAGE(...) ESP_LOGI("STORAGE", __VA_ARGS__)
#define LOGW_STORAGE(...) ESP_LOGW("STORAGE", __VA_ARGS__)
#define LOGE_STORAGE(...) ESP_LOGE("STORAGE", __VA_ARGS__)
#else
#define LOGI_STORAGE(...)
#define LOGW_STORAGE(...)
#define LOGE_STORAGE(...)
#endif

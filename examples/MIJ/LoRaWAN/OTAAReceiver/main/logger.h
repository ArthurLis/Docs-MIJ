#pragma once

#include "esp_log.h"
#include "config.h"

#ifdef DBG_MAIN
#define LOGI_MAIN(fmt, ...) ESP_LOGI("MAIN", fmt, ##__VA_ARGS__)
#define LOGW_MAIN(fmt, ...) ESP_LOGW("MAIN", fmt, ##__VA_ARGS__)
#define LOGE_MAIN(fmt, ...) ESP_LOGE("MAIN", fmt, ##__VA_ARGS__)
#define LOGD_MAIN(fmt, ...) ESP_LOGD("MAIN", fmt, ##__VA_ARGS__)
#else
#define LOGI_MAIN(...) do {} while (0)
#define LOGW_MAIN(...) do {} while (0)
#define LOGE_MAIN(...) do {} while (0)
#define LOGD_MAIN(...) do {} while (0)
#endif

#ifdef DBG_LORAWAN
#define LOGI_LORAWAN(fmt, ...) ESP_LOGI("LORAWAN", fmt, ##__VA_ARGS__)
#define LOGW_LORAWAN(fmt, ...) ESP_LOGW("LORAWAN", fmt, ##__VA_ARGS__)
#define LOGE_LORAWAN(fmt, ...) ESP_LOGE("LORAWAN", fmt, ##__VA_ARGS__)
#define LOGD_LORAWAN(fmt, ...) ESP_LOGD("LORAWAN", fmt, ##__VA_ARGS__)
#else
#define LOGI_LORAWAN(...) do {} while (0)
#define LOGW_LORAWAN(...) do {} while (0)
#define LOGE_LORAWAN(...) do {} while (0)
#define LOGD_LORAWAN(...) do {} while (0)
#endif

#ifdef DBG_STORAGE
#define LOGI_STORAGE(fmt, ...) ESP_LOGI("STORAGE", fmt, ##__VA_ARGS__)
#define LOGW_STORAGE(fmt, ...) ESP_LOGW("STORAGE", fmt, ##__VA_ARGS__)
#define LOGE_STORAGE(fmt, ...) ESP_LOGE("STORAGE", fmt, ##__VA_ARGS__)
#define LOGD_STORAGE(fmt, ...) ESP_LOGD("STORAGE", fmt, ##__VA_ARGS__)
#else
#define LOGI_STORAGE(...) do {} while (0)
#define LOGW_STORAGE(...) do {} while (0)
#define LOGE_STORAGE(...) do {} while (0)
#define LOGD_STORAGE(...) do {} while (0)
#endif

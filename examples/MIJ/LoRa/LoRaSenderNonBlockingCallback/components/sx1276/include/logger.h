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

#ifdef DBG_LORA
#define LOGI_LORA(fmt, ...) ESP_LOGI("LORA", fmt, ##__VA_ARGS__)
#define LOGW_LORA(fmt, ...) ESP_LOGW("LORA", fmt, ##__VA_ARGS__)
#define LOGE_LORA(fmt, ...) ESP_LOGE("LORA", fmt, ##__VA_ARGS__)
#define LOGD_LORA(fmt, ...) ESP_LOGD("LORA", fmt, ##__VA_ARGS__)
#else
#define LOGI_LORA(...) do {} while (0)
#define LOGW_LORA(...) do {} while (0)
#define LOGE_LORA(...) do {} while (0)
#define LOGD_LORA(...) do {} while (0)
#endif

#ifdef DBG_TEST
#define LOGI_TEST(fmt, ...) ESP_LOGI("TEST", fmt, ##__VA_ARGS__)
#define LOGW_TEST(fmt, ...) ESP_LOGW("TEST", fmt, ##__VA_ARGS__)
#define LOGE_TEST(fmt, ...) ESP_LOGE("TEST", fmt, ##__VA_ARGS__)
#define LOGD_TEST(fmt, ...) ESP_LOGD("TEST", fmt, ##__VA_ARGS__)
#else
#define LOGI_TEST(...) do {} while (0)
#define LOGW_TEST(...) do {} while (0)
#define LOGE_TEST(...) do {} while (0)
#define LOGD_TEST(...) do {} while (0)
#endif

#ifdef DBG_CONSOLE
#define LOGI_CONSOLE(fmt, ...) ESP_LOGI("CONSOLE", fmt, ##__VA_ARGS__)
#define LOGW_CONSOLE(fmt, ...) ESP_LOGW("CONSOLE", fmt, ##__VA_ARGS__)
#define LOGE_CONSOLE(fmt, ...) ESP_LOGE("CONSOLE", fmt, ##__VA_ARGS__)
#else
#define LOGI_CONSOLE(...) do {} while (0)
#define LOGW_CONSOLE(...) do {} while (0)
#define LOGE_CONSOLE(...) do {} while (0)
#endif

#ifdef DBG_STORAGE
#define LOGI_STORAGE(fmt, ...) ESP_LOGI("STORAGE", fmt, ##__VA_ARGS__)
#define LOGW_STORAGE(fmt, ...) ESP_LOGW("STORAGE", fmt, ##__VA_ARGS__)
#define LOGE_STORAGE(fmt, ...) ESP_LOGE("STORAGE", fmt, ##__VA_ARGS__)
#else
#define LOGI_STORAGE(...) do {} while (0)
#define LOGW_STORAGE(...) do {} while (0)
#define LOGE_STORAGE(...) do {} while (0)
#endif

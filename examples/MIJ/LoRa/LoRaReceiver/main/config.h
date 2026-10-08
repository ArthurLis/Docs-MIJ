#pragma once

#include <cstddef>
#include <cstdint>
#include "driver/gpio.h"
#include "driver/spi_master.h"

/*
 * JVTECH - SX1276 ESP-IDF example
 * ESP-IDF: 5.3.1
 * Target: ESP32
 *
 * Change the LoRa GPIOs only in this file.
 */

#define DBG_MAIN
#define DBG_LORA

namespace appcfg {

constexpr const char *FIRMWARE_NAME = "LoRaReceiver";
constexpr const char *FIRMWARE_VERSION = "1.0.0";
constexpr const char *ESP_IDF_VERSION_EXPECTED = "5.3.1";

// Example pinout. Adjust to your hardware.
constexpr spi_host_device_t LORA_SPI_HOST = SPI2_HOST;
constexpr gpio_num_t LORA_PIN_SCLK = GPIO_NUM_18;
constexpr gpio_num_t LORA_PIN_MISO = GPIO_NUM_19;
constexpr gpio_num_t LORA_PIN_MOSI = GPIO_NUM_23;
constexpr gpio_num_t LORA_PIN_NSS  = GPIO_NUM_5;
constexpr gpio_num_t LORA_PIN_RST  = GPIO_NUM_14;
constexpr gpio_num_t LORA_PIN_DIO0 = GPIO_NUM_26;
constexpr gpio_num_t LORA_PIN_DIO1 = GPIO_NUM_13;
constexpr gpio_num_t LORA_PIN_DIO2 = GPIO_NUM_32;

constexpr int LORA_SPI_CLOCK_HZ = 8'000'000;
constexpr uint32_t LORA_XTAL_HZ = 32'000'000;
constexpr uint8_t SX1276_EXPECTED_VERSION = 0x12;

constexpr uint32_t DEFAULT_FREQUENCY_HZ = 915'000'000;
constexpr uint8_t DEFAULT_BANDWIDTH_CODE = 7;      // 125 kHz
constexpr uint8_t DEFAULT_SPREADING_FACTOR = 7;    // SF7
constexpr uint8_t DEFAULT_CODING_RATE = 1;         // 4/5
constexpr int8_t DEFAULT_TX_POWER_DBM = 17;
constexpr uint16_t DEFAULT_PREAMBLE_SYMBOLS = 8;
constexpr bool DEFAULT_CRC_ENABLED = true;
constexpr bool DEFAULT_IMPLICIT_HEADER = false;
constexpr uint8_t DEFAULT_SYNC_WORD = 0x12;
constexpr bool DEFAULT_IQ_INVERTED = false;
constexpr bool DEFAULT_PA_BOOST = true;
constexpr uint16_t DEFAULT_RX_SYMBOL_TIMEOUT = 100;
constexpr uint8_t DEFAULT_IMPLICIT_PAYLOAD_LENGTH = 32;

constexpr uint32_t RADIO_TX_TIMEOUT_MS = 10'000;
constexpr uint32_t RADIO_RX_DEFAULT_TIMEOUT_MS = 5'000;
constexpr uint32_t RADIO_POLL_INTERVAL_MS = 2;
constexpr size_t RADIO_MAX_PAYLOAD = 255;

constexpr uint32_t MIN_FREQUENCY_HZ = 137'000'000;
constexpr uint32_t MAX_FREQUENCY_HZ = 1'020'000'000;

} // namespace appcfg

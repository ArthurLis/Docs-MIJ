#pragma once

#include <cstddef>
#include <cstdint>
#include "RadioLib.h"
#include "driver/spi_master.h"

/*
 * JVTECH - LoRaReceiverCallback
 * Target: ESP32-C6 + SX1262
 * ESP-IDF: 5.3.1
 * Radio: raw LoRa
 *
 * IMPORTANT: GPIOs below are GENERIC EXAMPLE PINS.
 * Change them to match the actual board before wiring or flashing.
 */

#define DBG_MAIN
#define DBG_LORA

namespace appcfg {

constexpr const char *FIRMWARE_NAME = "LoRaReceiverCallback";
constexpr const char *FIRMWARE_VERSION = "1.0.0";
constexpr const char *ESP_IDF_VERSION_EXPECTED = "5.3.1";

// -----------------------------------------------------------------------------
// Generic ESP32-C6 <-> SX1262 wiring. CHANGE FOR YOUR BOARD.
// SX1262 needs DIO1 and BUSY. DIO0 is not used by SX1262.
// DIO2 is normally controlled internally by the radio as the RF switch.
// -----------------------------------------------------------------------------
constexpr spi_host_device_t LORA_SPI_HOST = SPI2_HOST;
constexpr int LORA_PIN_SCLK = 6;   // example only
constexpr int LORA_PIN_MISO = 5;   // example only
constexpr int LORA_PIN_MOSI = 7;   // example only
constexpr int LORA_PIN_NSS  = 4;  // example only
constexpr int LORA_PIN_RST  = 1;   // example only
constexpr int LORA_PIN_DIO1 = 0;   // example only - IRQ
constexpr int LORA_PIN_BUSY = 2;   // example only
constexpr uint32_t LORA_SPI_CLOCK_HZ = 8'000'000;

// Most SX1262 modules use a TCXO controlled through DIO3.
// Use 0.0F if the module has a plain XTAL instead of a TCXO.
constexpr float SX1262_TCXO_VOLTAGE = 1.6F;
constexpr bool SX1262_USE_REGULATOR_LDO = false;
constexpr bool SX1262_USE_DIO2_RF_SWITCH = true;

// Raw LoRa settings - sender and receiver MUST match.
constexpr float LORA_FREQUENCY_MHZ = 915.0F;
constexpr float LORA_BANDWIDTH_KHZ = 125.0F;
constexpr uint8_t LORA_SPREADING_FACTOR = 7;
constexpr uint8_t LORA_CODING_RATE = 5;       // 4/5
constexpr uint8_t LORA_SYNC_WORD = 0x12;      // private/raw LoRa
constexpr int8_t LORA_TX_POWER_DBM = 17;
constexpr uint16_t LORA_PREAMBLE_LENGTH = 8;

constexpr uint32_t SEND_INTERVAL_MS = 5'000;
constexpr uint32_t MAIN_WORK_INTERVAL_MS = 100;
constexpr size_t MAX_PACKET_BYTES = 255;

} // namespace appcfg

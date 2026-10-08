#pragma once

#include <cstddef>
#include <cstdint>
#include "RadioLib.h"
#include "driver/spi_master.h"

/*
 * JVTECH - OTAASender
 * Target: ESP32-C6 + SX1262
 * ESP-IDF: 5.3.1
 * LoRaWAN: Class A
 *
 * GPIOs are generic example values. Change them to match the real board.
 * Credentials are HEX strings: copy from the network server and paste between quotes.
 */

#define DBG_MAIN
#define DBG_LORAWAN
#define DBG_STORAGE
#define LORAWAN_EXAMPLE_OTAA

namespace appcfg {

constexpr const char *FIRMWARE_NAME = "OTAASender";
constexpr const char *FIRMWARE_VERSION = "1.0.0";
constexpr const char *ESP_IDF_VERSION_EXPECTED = "5.3.1";

// -----------------------------------------------------------------------------
// Generic ESP32-C6 <-> SX1262 wiring. CHANGE FOR YOUR BOARD.
// SX1262: NSS + DIO1 + NRST + BUSY are required by RadioLib.
// DIO2 is used internally for RF-switch control; DIO3 can control a TCXO.
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

constexpr float SX1262_TCXO_VOLTAGE = 1.6F; // set 0.0F for XTAL modules
constexpr bool SX1262_USE_REGULATOR_LDO = false;
constexpr bool SX1262_USE_DIO2_RF_SWITCH = true;

// -----------------------------------------------------------------------------
// LoRaWAN region/channel plan.
// Change sub-band to match the network server/gateway channel plan.
// -----------------------------------------------------------------------------
constexpr const LoRaWANBand_t *LORAWAN_BAND = &AU915;
constexpr uint8_t LORAWAN_SUB_BAND = 1;
constexpr bool ENABLE_ADR = true;
constexpr uint8_t DEFAULT_UPLINK_DR = 3;

// Initial SX1262 modem configuration. LoRaWAN changes channels/DR afterwards.
constexpr float RADIO_INITIAL_FREQUENCY_MHZ = 915.0F;
constexpr float RADIO_INITIAL_BANDWIDTH_KHZ = 125.0F;
constexpr uint8_t RADIO_INITIAL_SPREADING_FACTOR = 7;
constexpr uint8_t RADIO_INITIAL_CODING_RATE = 5;
constexpr uint8_t LORAWAN_SYNC_WORD = 0x34;
constexpr int8_t DEFAULT_TX_POWER_DBM = 17;
constexpr uint16_t RADIO_PREAMBLE_LENGTH = 8;

// -----------------------------------------------------------------------------
// ABP credentials - copy/paste HEX strings.
// LoRaWAN 1.0.x: DevAddr + NwkSKey + AppSKey.
// -----------------------------------------------------------------------------
constexpr char ABP_DEV_ADDR_HEX[] = "00000000";
constexpr char ABP_NWK_S_KEY_HEX[] = "00000000000000000000000000000000";
constexpr char ABP_APP_S_KEY_HEX[] = "00000000000000000000000000000000";

// Optional ABP LoRaWAN 1.1. Leave ALL THREE empty for 1.0.x.
constexpr char ABP_F_NWK_S_INT_KEY_HEX[] = "";
constexpr char ABP_S_NWK_S_INT_KEY_HEX[] = "";
constexpr char ABP_NWK_S_ENC_KEY_HEX[] = "";

// -----------------------------------------------------------------------------
// OTAA credentials - copy/paste HEX strings.
// NwkKey empty = LoRaWAN 1.0.x; populated = LoRaWAN 1.1.
// -----------------------------------------------------------------------------
constexpr char OTAA_JOIN_EUI_HEX[] = "2413984507c63288";
constexpr char OTAA_DEV_EUI_HEX[]  = "c92874ba3b476042";
constexpr char OTAA_APP_KEY_HEX[]  = "1da1e769cefeb68f13456c657fc66893";
constexpr char OTAA_NWK_KEY_HEX[]  = "";

// Example behavior.
constexpr uint8_t UPLINK_FPORT = 10;
constexpr bool UPLINK_CONFIRMED = false;
constexpr size_t EXAMPLE_PAYLOAD_SIZE = 32;
constexpr uint32_t SEND_INTERVAL_MS = 60'000;
constexpr uint32_t DOWNLINK_POLL_INTERVAL_MS = 30'000;
constexpr uint32_t OTAA_JOIN_RETRY_MS = 30'000;
constexpr size_t MAX_DOWNLINK_BYTES = 255;

constexpr const char *ABP_NVS_NAMESPACE = "lw_abp";
constexpr const char *OTAA_NVS_NAMESPACE = "lw_otaa";

} // namespace appcfg

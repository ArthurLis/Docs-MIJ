#pragma once

#include <cstddef>
#include <cstdint>
#include "RadioLib.h"
#include "driver/spi_master.h"

/*
 * JVTECH - OTAASender
 * Target: ESP32 + SX1276
 * ESP-IDF: 5.3.1
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
// Hardware - change only these GPIOs for your board. DIO2 is NOT used.
// -----------------------------------------------------------------------------
constexpr spi_host_device_t LORA_SPI_HOST = SPI2_HOST;
constexpr int LORA_PIN_SCLK = 18;
constexpr int LORA_PIN_MISO = 19;
constexpr int LORA_PIN_MOSI = 23;
constexpr int LORA_PIN_NSS  = 5;
constexpr int LORA_PIN_RST  = 14;
constexpr int LORA_PIN_DIO0 = 26;
constexpr int LORA_PIN_DIO1 = 13;
constexpr uint32_t LORA_SPI_CLOCK_HZ = 8'000'000;

// -----------------------------------------------------------------------------
// LoRaWAN region/channel plan.
// Default: AU915 sub-band 1. MUST match the network server/gateway channel plan.
// Other examples: &US915, &EU868, &AS923.
// -----------------------------------------------------------------------------
constexpr const LoRaWANBand_t *LORAWAN_BAND = &AU915;
constexpr uint8_t LORAWAN_SUB_BAND = 1;
constexpr bool ENABLE_ADR = true;

// Initial SX1276 modem settings. LoRaWAN changes channel/DR as required later.
constexpr float RADIO_INITIAL_FREQUENCY_MHZ = 915.0F;
constexpr float RADIO_INITIAL_BANDWIDTH_KHZ = 125.0F;
constexpr uint8_t RADIO_INITIAL_SPREADING_FACTOR = 7;
constexpr uint8_t RADIO_INITIAL_CODING_RATE = 5; // 4/5
constexpr uint8_t LORAWAN_SYNC_WORD = 0x34;
constexpr int8_t DEFAULT_TX_POWER_DBM = 17;
constexpr uint16_t RADIO_PREAMBLE_LENGTH = 8;

// -----------------------------------------------------------------------------
// OTAA credentials.
// COPY -> PASTE. Do NOT convert to {0x.., 0x..}.
// JoinEUI (old AppEUI) may legitimately be all zeros on some networks.
// -----------------------------------------------------------------------------
constexpr char OTAA_JOIN_EUI_HEX[] = "0000000000000000";
constexpr char OTAA_DEV_EUI_HEX[] = "0000000000000000";
constexpr char OTAA_APP_KEY_HEX[] = "00000000000000000000000000000000";
constexpr char OTAA_NWK_KEY_HEX[] = "";

// -----------------------------------------------------------------------------
// Example behavior.
// -----------------------------------------------------------------------------
constexpr uint8_t UPLINK_FPORT = 10;
constexpr bool UPLINK_CONFIRMED = false;
constexpr uint32_t SEND_INTERVAL_MS = 60'000;
constexpr uint32_t OTAA_JOIN_RETRY_MS = 5'000;
constexpr size_t MAX_DOWNLINK_BYTES = 255;

// Sender/Receiver of the same activation mode share NVS state intentionally.
constexpr const char *OTAA_NVS_NAMESPACE = "lw_otaa";

} // namespace appcfg

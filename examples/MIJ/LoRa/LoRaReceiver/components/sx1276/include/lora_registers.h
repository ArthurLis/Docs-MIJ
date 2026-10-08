#pragma once

#include <cstdint>

namespace sx1276 {

constexpr uint8_t REG_FIFO = 0x00;
constexpr uint8_t REG_OP_MODE = 0x01;
constexpr uint8_t REG_FRF_MSB = 0x06;
constexpr uint8_t REG_FRF_MID = 0x07;
constexpr uint8_t REG_FRF_LSB = 0x08;
constexpr uint8_t REG_PA_CONFIG = 0x09;
constexpr uint8_t REG_PA_RAMP = 0x0A;
constexpr uint8_t REG_OCP = 0x0B;
constexpr uint8_t REG_LNA = 0x0C;
constexpr uint8_t REG_FIFO_ADDR_PTR = 0x0D;
constexpr uint8_t REG_FIFO_TX_BASE_ADDR = 0x0E;
constexpr uint8_t REG_FIFO_RX_BASE_ADDR = 0x0F;
constexpr uint8_t REG_FIFO_RX_CURRENT_ADDR = 0x10;
constexpr uint8_t REG_IRQ_FLAGS_MASK = 0x11;
constexpr uint8_t REG_IRQ_FLAGS = 0x12;
constexpr uint8_t REG_RX_NB_BYTES = 0x13;
constexpr uint8_t REG_RX_HEADER_CNT_VALUE_MSB = 0x14;
constexpr uint8_t REG_RX_HEADER_CNT_VALUE_LSB = 0x15;
constexpr uint8_t REG_RX_PACKET_CNT_VALUE_MSB = 0x16;
constexpr uint8_t REG_RX_PACKET_CNT_VALUE_LSB = 0x17;
constexpr uint8_t REG_MODEM_STAT = 0x18;
constexpr uint8_t REG_PKT_SNR_VALUE = 0x19;
constexpr uint8_t REG_PKT_RSSI_VALUE = 0x1A;
constexpr uint8_t REG_RSSI_VALUE = 0x1B;
constexpr uint8_t REG_HOP_CHANNEL = 0x1C;
constexpr uint8_t REG_MODEM_CONFIG_1 = 0x1D;
constexpr uint8_t REG_MODEM_CONFIG_2 = 0x1E;
constexpr uint8_t REG_SYMB_TIMEOUT_LSB = 0x1F;
constexpr uint8_t REG_PREAMBLE_MSB = 0x20;
constexpr uint8_t REG_PREAMBLE_LSB = 0x21;
constexpr uint8_t REG_PAYLOAD_LENGTH = 0x22;
constexpr uint8_t REG_MAX_PAYLOAD_LENGTH = 0x23;
constexpr uint8_t REG_HOP_PERIOD = 0x24;
constexpr uint8_t REG_FIFO_RX_BYTE_ADDR = 0x25;
constexpr uint8_t REG_MODEM_CONFIG_3 = 0x26;
constexpr uint8_t REG_PPM_CORRECTION = 0x27;
constexpr uint8_t REG_FEI_MSB = 0x28;
constexpr uint8_t REG_FEI_MID = 0x29;
constexpr uint8_t REG_FEI_LSB = 0x2A;
constexpr uint8_t REG_RSSI_WIDEBAND = 0x2C;
constexpr uint8_t REG_IF_FREQ_1 = 0x2F;
constexpr uint8_t REG_IF_FREQ_2 = 0x30;
constexpr uint8_t REG_DETECT_OPTIMIZE = 0x31;
constexpr uint8_t REG_INVERT_IQ = 0x33;
constexpr uint8_t REG_HIGH_BW_OPTIMIZE_1 = 0x36;
constexpr uint8_t REG_DETECTION_THRESHOLD = 0x37;
constexpr uint8_t REG_SYNC_WORD = 0x39;
constexpr uint8_t REG_HIGH_BW_OPTIMIZE_2 = 0x3A;
constexpr uint8_t REG_INVERT_IQ_2 = 0x3B;
constexpr uint8_t REG_DIO_MAPPING_1 = 0x40;
constexpr uint8_t REG_DIO_MAPPING_2 = 0x41;
constexpr uint8_t REG_VERSION = 0x42;
constexpr uint8_t REG_PLL_HOP = 0x44;
constexpr uint8_t REG_PA_DAC = 0x4D;

constexpr uint8_t OPMODE_LONG_RANGE = 0x80;
constexpr uint8_t OPMODE_LOW_FREQUENCY = 0x08;
constexpr uint8_t OPMODE_MODE_MASK = 0x07;

constexpr uint8_t IRQ_RX_TIMEOUT = 0x80;
constexpr uint8_t IRQ_RX_DONE = 0x40;
constexpr uint8_t IRQ_PAYLOAD_CRC_ERROR = 0x20;
constexpr uint8_t IRQ_VALID_HEADER = 0x10;
constexpr uint8_t IRQ_TX_DONE = 0x08;
constexpr uint8_t IRQ_CAD_DONE = 0x04;
constexpr uint8_t IRQ_FHSS_CHANGE_CHANNEL = 0x02;
constexpr uint8_t IRQ_CAD_DETECTED = 0x01;

constexpr uint8_t DIO0_RX_DONE = 0x00;
constexpr uint8_t DIO0_TX_DONE = 0x40;
constexpr uint8_t DIO0_CAD_DONE = 0x80;
constexpr uint8_t DIO0_MASK = 0x3F;
constexpr uint8_t DIO1_RX_TIMEOUT = 0x00;
constexpr uint8_t DIO1_FHSS_CHANGE = 0x10;
constexpr uint8_t DIO1_CAD_DETECTED = 0x20;
constexpr uint8_t DIO1_MASK = 0xCF;

constexpr uint8_t PA_BOOST = 0x80;
constexpr uint8_t PA_DAC_DEFAULT = 0x84;
constexpr uint8_t PA_DAC_20_DBM = 0x87;

constexpr uint8_t INVERT_IQ_RX_MASK = 0xBF;
constexpr uint8_t INVERT_IQ_RX_OFF = 0x00;
constexpr uint8_t INVERT_IQ_RX_ON = 0x40;
constexpr uint8_t INVERT_IQ_TX_MASK = 0xFE;
constexpr uint8_t INVERT_IQ_TX_OFF = 0x01;
constexpr uint8_t INVERT_IQ_TX_ON = 0x00;
constexpr uint8_t INVERT_IQ_2_ON = 0x19;
constexpr uint8_t INVERT_IQ_2_OFF = 0x1D;

constexpr uint8_t DETECT_OPTIMIZE_SF7_TO_SF12 = 0x03;
constexpr uint8_t DETECT_OPTIMIZE_SF6 = 0x05;
constexpr uint8_t DETECTION_THRESHOLD_SF7_TO_SF12 = 0x0A;
constexpr uint8_t DETECTION_THRESHOLD_SF6 = 0x0C;

} // namespace sx1276

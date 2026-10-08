#pragma once

#include <cstddef>
#include <cstdint>

#include "driver/spi_master.h"
#include "lora_types.h"

class Sx1276Radio {
public:
    Sx1276Radio();

    lora::RadioResult begin(const lora::RadioSettings &settings);
    lora::RadioResult reset();
    lora::RadioResult applySettings(const lora::RadioSettings &settings);

    lora::RadioResult setFrequency(uint32_t frequencyHz);
    lora::RadioResult setBandwidth(uint8_t bandwidthCode);
    lora::RadioResult setSpreadingFactor(uint8_t spreadingFactor);
    lora::RadioResult setCodingRate(uint8_t codingRate);
    lora::RadioResult setTxPower(int8_t powerDbm, bool paBoost);
    lora::RadioResult setPreamble(uint16_t symbols);
    lora::RadioResult setCrc(bool enabled);
    lora::RadioResult setHeaderMode(bool implicitHeader);
    lora::RadioResult setSyncWord(uint8_t syncWord);
    lora::RadioResult setIqInverted(bool inverted);
    lora::RadioResult setRxSymbolTimeout(uint16_t symbols);
    lora::RadioResult setImplicitPayloadLength(uint8_t length);
    lora::RadioResult setMode(lora::RadioMode mode);

    lora::RadioResult sleep();
    lora::RadioResult standby();
    lora::RadioResult transmit(const uint8_t *data, size_t length, uint32_t timeoutMs);
    lora::RadioResult receive(uint8_t *buffer, size_t capacity, lora::PacketInfo &info, uint32_t timeoutMs);
    lora::RadioResult startReceiveContinuous();
    lora::RadioResult pollReceivedPacket(uint8_t *buffer, size_t capacity, lora::PacketInfo &info);
    lora::RadioResult performCad(lora::CadResult &result, uint32_t timeoutMs);
    lora::RadioResult runRxSingleTimeoutTest(bool &rxTimeoutSeen, uint32_t timeoutMs);

    float readCurrentRssiDbm();
    float readPacketSnrDb();
    float readPacketRssiDbm();
    float readFrequencyErrorHz();
    uint8_t readWidebandRssi();

    lora::RadioResult readRegister(uint8_t address, uint8_t &value);
    lora::RadioResult writeRegister(uint8_t address, uint8_t value);
    lora::RadioResult readBuffer(uint8_t address, uint8_t *data, size_t length);
    lora::RadioResult writeBuffer(uint8_t address, const uint8_t *data, size_t length);

    lora::RadioResult setTxContinuousMode(bool enabled);
    lora::RadioResult setHopPeriod(uint8_t symbols);
    lora::RadioResult setFastHop(bool enabled);

    uint8_t getVersion();
    uint8_t getIrqFlags();
    uint16_t getRxHeaderCount();
    uint16_t getRxPacketCount();
    uint32_t getFrequencyHz();
    uint32_t getDio0InterruptCount() const;
    uint32_t getDio1InterruptCount() const;

    double calculateTimeOnAirMs(size_t payloadLength) const;
    const lora::RadioSettings &getSettings() const;
    bool isInitialized() const;

    void dumpRegisters();

private:
    lora::RadioResult initializeSpi();
    lora::RadioResult initializeGpio();
    lora::RadioResult transfer(const uint8_t *tx, uint8_t *rx, size_t length);
    lora::RadioResult clearIrqFlags(uint8_t mask = 0xFF);
    lora::RadioResult configureLna();
    lora::RadioResult configureRxErrata(uint32_t &originalFrequencyHz);
    void restoreRxFrequency(uint32_t originalFrequencyHz);
    lora::RadioResult configureIqForTx();
    lora::RadioResult configureIqForRx();
    void updateLowDataRateOptimize();
    void updateHighBandwidthOptimization();
    bool validateSettings(const lora::RadioSettings &settings) const;
    int16_t getRssiOffsetDb() const;
    static void handleDio0Isr(void *arg);
    static void handleDio1Isr(void *arg);

    spi_device_handle_t spiDevice_;
    lora::RadioSettings settings_;
    bool spiInitialized_;
    bool initialized_;
    volatile uint32_t dio0InterruptCount_;
    volatile uint32_t dio1InterruptCount_;
};

const char *radioResultToString(lora::RadioResult result);

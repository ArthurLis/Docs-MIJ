# LoRaReceiverCallback - ESP32-C6 + SX1262

Example version **1.0.0** for ESP-IDF **5.3.1** using RadioLib **7.8.1** through the ESP-IDF Component Manager.

The GPIO values in `main/config.h` are generic placeholders. Change SCLK, MISO, MOSI, NSS, RST, DIO1 and BUSY to match the real board. SX1262 does not use DIO0; DIO2 is configured by the radio for RF-switch control.

Build:

```bash
idf.py set-target esp32c6
idf.py build
idf.py flash monitor
```

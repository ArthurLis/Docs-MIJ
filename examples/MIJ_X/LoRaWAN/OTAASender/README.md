# OTAASender - ESP32-C6 + SX1262 LoRaWAN

Independent ESP-IDF **5.3.1** example using RadioLib **7.8.1** and LoRaWAN Class A.

- GPIOs in `main/config.h` are generic placeholders.
- SX1262 uses **DIO1 + BUSY**, not the SX1276 DIO0/DIO1 pair.
- TCXO voltage is configurable (`1.6V` default; use `0.0V` for XTAL modules).
- ABP/OTAA keys are plain HEX strings for copy/paste.
- Initial application uplink DR is **DR3** and sender payload target is **32 bytes**. ADR remains enabled.

Build:

```bash
idf.py set-target esp32c6
idf.py build
idf.py flash monitor
```

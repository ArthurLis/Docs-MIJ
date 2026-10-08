# Configuração LoRaWAN

## ABP

Cole as credenciais no `config.h` usando strings HEX.

```cpp
constexpr char ABP_DEV_ADDR_HEX[] = "00000000";
constexpr char ABP_NWK_S_KEY_HEX[] = "00000000000000000000000000000000";
constexpr char ABP_APP_S_KEY_HEX[] = "00000000000000000000000000000000";
```

## OTAA

```cpp
constexpr char OTAA_JOIN_EUI_HEX[] = "0000000000000000";
constexpr char OTAA_DEV_EUI_HEX[]  = "0000000000000000";
constexpr char OTAA_APP_KEY_HEX[]  = "00000000000000000000000000000000";
constexpr char OTAA_NWK_KEY_HEX[]  = "";
```
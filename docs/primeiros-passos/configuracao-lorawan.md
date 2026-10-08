# Configuração LoRaWAN

Os exemplos utilizam **LoRaWAN Class A**. Além das credenciais, a região e o sub-band devem ser compatíveis com o servidor de rede e com o gateway.

> [!IMPORTANT]
> Os valores abaixo são apenas placeholders. Substitua-os pelas credenciais do seu dispositivo antes do teste e nunca publique chaves reais no repositório.

## ABP

No ABP, o dispositivo já recebe o endereço e as chaves de sessão previamente.

```cpp
constexpr char ABP_DEV_ADDR_HEX[] = "00000000";
constexpr char ABP_NWK_S_KEY_HEX[] = "00000000000000000000000000000000";
constexpr char ABP_APP_S_KEY_HEX[] = "00000000000000000000000000000000";
```

## OTAA

No OTAA, o dispositivo realiza um Join com a rede antes de iniciar a comunicação.

```cpp
constexpr char OTAA_JOIN_EUI_HEX[] = "0000000000000000";
constexpr char OTAA_DEV_EUI_HEX[]  = "0000000000000000";
constexpr char OTAA_APP_KEY_HEX[]  = "00000000000000000000000000000000";
constexpr char OTAA_NWK_KEY_HEX[]  = "";
```

`OTAA_NWK_KEY_HEX` permanece vazio nos exemplos LoRaWAN 1.0.x. Para LoRaWAN 1.1, utilize a chave correspondente fornecida pelo servidor de rede.

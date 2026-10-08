# OTAASender

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRaWAN Class A / OTAA  
> **Projeto:** `examples/MIJ/LoRaWAN/OTAASender`

## Objetivo

Realiza ativação OTAA e envia uplinks periódicos.

## Funcionamento

O dispositivo carrega JoinEUI, DevEUI e AppKey, tenta realizar o Join e salva a sessão em NVS. Depois do Join, envia uplinks periódicos e abre RX1/RX2 após cada envio.

## Hardware

Use a MIJ com ESP32 + SX1276. A pinagem padrão é:

| Sinal | GPIO |
| --- | ---: |
| SCLK | 18 |
| MISO | 19 |
| MOSI | 23 |
| NSS / CS | 5 |
| RST | 14 |
| DIO0 | 26 |
| DIO1 | 13 |

O DIO2 não é utilizado.

## Configuração

Arquivo principal: `main/config.h`.

Preencha somente as credenciais do modo deste exemplo e confira `LORAWAN_BAND` e `LORAWAN_SUB_BAND` para que correspondam à rede.

## Como testar

Cadastre o dispositivo OTAA no servidor, preencha as credenciais, grave o firmware e acompanhe primeiro o Join e depois os uplinks.

## Resultado esperado

O monitor deve confirmar a ativação OTAA e, em seguida, mostrar os uplinks enviados.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha JoinEUI, DevEUI e AppKey antes do teste e não publique chaves reais. O estado da sessão e os contadores são persistidos em NVS.

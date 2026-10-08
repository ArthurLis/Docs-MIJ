# ABPSender

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRaWAN Class A / ABP  
> **Projeto:** `examples/MIJ/LoRaWAN/ABPSender`

## Objetivo

Ativa uma sessão ABP e envia uplinks periódicos.

## Funcionamento

O dispositivo carrega as credenciais ABP, restaura o estado salvo em NVS e ativa a sessão. Depois envia pacotes em intervalos regulares e abre RX1/RX2 após cada uplink.

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

Cadastre o dispositivo no servidor de rede, preencha DevAddr, NwkSKey e AppSKey, grave o firmware e acompanhe os uplinks no servidor.

## Resultado esperado

O monitor deve indicar a ativação ABP e, a cada envio, mostrar FCnt, FPort, DR, frequência e potência.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha-as somente no ambiente de teste e não publique chaves reais. O estado da sessão e os contadores são persistidos em NVS.

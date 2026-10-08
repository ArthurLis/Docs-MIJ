# OTAAReceiver

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRaWAN Class A / OTAA  
> **Projeto:** `examples/MIJ/LoRaWAN/OTAAReceiver`

## Objetivo

Realiza Join OTAA e demonstra o recebimento de downlinks.

## Funcionamento

Depois de ativar a sessão OTAA, o exemplo envia um uplink vazio periodicamente. Esse uplink abre RX1/RX2, onde um downlink pendente pode ser recebido.

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

Cadastre o dispositivo OTAA, preencha as credenciais, aguarde o Join, coloque um downlink na fila do servidor e observe o próximo ciclo RX1/RX2.

## Resultado esperado

Quando houver downlink, o monitor deve mostrar a janela utilizada e os dados recebidos.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha JoinEUI, DevEUI e AppKey antes do teste e não publique chaves reais. Em Class A, downlinks só são recebidos nas janelas abertas depois de um uplink. O estado da sessão e os contadores são persistidos em NVS.

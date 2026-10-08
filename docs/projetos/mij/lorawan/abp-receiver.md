# ABPReceiver

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRaWAN Class A / ABP  
> **Projeto:** `examples/MIJ/LoRaWAN/ABPReceiver`

## Objetivo

Demonstra o recebimento de downlinks em uma sessão ABP.

## Funcionamento

Em Class A o dispositivo não fica ouvindo o tempo todo. Este exemplo envia um uplink vazio apenas para abrir as janelas RX1 e RX2 e verificar se existe um downlink pendente.

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

Cadastre o dispositivo em ABP, preencha as credenciais, coloque um downlink na fila do servidor e execute o exemplo.

## Resultado esperado

Se houver downlink pendente, o monitor deve informar RX1 ou RX2, FPort, FCnt, RSSI, SNR e o payload em HEX.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha-as somente no ambiente de teste e não publique chaves reais. Em Class A, downlinks só são recebidos nas janelas abertas depois de um uplink. O estado da sessão e os contadores são persistidos em NVS.

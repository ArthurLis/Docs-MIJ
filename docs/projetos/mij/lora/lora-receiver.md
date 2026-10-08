# LoRaReceiver

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ/LoRa/LoRaReceiver`

## Objetivo

Recebe pacotes LoRa de forma simples.

## Funcionamento

O firmware consulta a recepção em ciclos de polling. Se um pacote válido chegar, ele registra conteúdo, RSSI e SNR. Timeouts são normais enquanto não existe transmissão.

## Hardware

A ligação padrão usada nestes exemplos é:

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

Ajuste frequência, bandwidth, spreading factor, coding rate, sync word e potência conforme o teste. Esse mesmo arquivo também contém a pinagem. O componente SX1276 reutiliza essa configuração.

## Como testar

Grave `LoRaSender` em outro dispositivo e mantenha os dois rádios com a mesma configuração.

## Resultado esperado

Cada pacote recebido deve aparecer no monitor com os dados e informações de sinal.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

# LoRaSenderNonBlockingCallback

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ/LoRa/LoRaSenderNonBlockingCallback`

## Objetivo

Demonstra transmissão LoRa não bloqueante.

## Funcionamento

O SX1276 inicia o envio e retorna imediatamente para a aplicação. O DIO0 sinaliza `TxDone`; depois disso o firmware encerra a transmissão e agenda o próximo pacote.

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

Use `LoRaReceiver` ou `LoRaReceiverCallback` no outro dispositivo.

## Resultado esperado

O log deve mostrar o início do envio e, depois, a confirmação do callback de transmissão concluída.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

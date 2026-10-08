# LoRaSenderNonBlockingCallback

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ_X/LoRa/LoRaSenderNonBlockingCallback`

## Objetivo

Demonstra transmissão LoRa não bloqueante.

## Funcionamento

A transmissão é iniciada com `startTransmit()`, permitindo que a tarefa principal continue executando. O DIO1 sinaliza `TxDone`; depois o firmware chama `finishTransmit()`.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Além dos parâmetros LoRa, ajuste os GPIOs do SX1262. Confira `SX1262_TCXO_VOLTAGE` e a configuração do RF switch conforme o módulo utilizado.

## Como testar

Use `LoRaReceiver` ou `LoRaReceiverCallback` no outro dispositivo.

## Resultado esperado

O log deve mostrar o início do envio e, depois, a confirmação do callback de transmissão concluída.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

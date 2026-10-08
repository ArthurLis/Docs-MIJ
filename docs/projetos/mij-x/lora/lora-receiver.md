# LoRaReceiver

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ_X/LoRa/LoRaReceiver`

## Objetivo

Recebe pacotes LoRa de forma simples.

## Funcionamento

O firmware usa a recepção bloqueante da RadioLib. Ele permanece aguardando um pacote e, quando recebe, registra conteúdo, RSSI e SNR.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Além dos parâmetros LoRa, ajuste os GPIOs do SX1262. Confira `SX1262_TCXO_VOLTAGE` e a configuração do RF switch conforme o módulo utilizado.

## Como testar

Grave `LoRaSender` em outro dispositivo e mantenha os dois rádios com a mesma configuração.

## Resultado esperado

Cada pacote recebido deve aparecer no monitor com os dados e informações de sinal.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

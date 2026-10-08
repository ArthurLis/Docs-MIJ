# MIJ_X — ESP32-C6 + SX1262

## Visão geral

A MIJ_X utiliza um **ESP32-C6** com rádio **SX1262**.

## Pinagem

Os exemplos deixam os GPIOs como valores genéricos. Antes de compilar, ajuste em `main/config.h` os sinais abaixo para a placa utilizada:

- SCLK
- MISO
- MOSI
- NSS / CS
- RST
- DIO1
- BUSY

O SX1262 usa **DIO1** para interrupções e possui o sinal **BUSY**, que não existe no fluxo usado pelo SX1276.

Também confira `SX1262_TCXO_VOLTAGE`: use o valor adequado ao módulo ou `0.0F` quando o hardware utilizar XTAL em vez de TCXO.

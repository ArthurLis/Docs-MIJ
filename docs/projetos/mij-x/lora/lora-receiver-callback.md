# LoRaReceiverCallback

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ_X/LoRa/LoRaReceiverCallback`

## Objetivo

Recebe pacotes LoRa usando uma interrupção/callback para avisar a aplicação quando uma recepção termina.

## Funcionamento

O SX1262 permanece em recepção contínua. O DIO1 dispara o callback quando o pacote termina; a aplicação então lê os dados e registra o resultado.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Além dos parâmetros LoRa, ajuste os GPIOs do SX1262. Confira `SX1262_TCXO_VOLTAGE` e a configuração do RF switch conforme o módulo utilizado.

## Como testar

Grave `LoRaSender` em outro dispositivo e envie pacotes com os mesmos parâmetros LoRa.

## Resultado esperado

O monitor deve indicar o evento de recepção e exibir o pacote recebido.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

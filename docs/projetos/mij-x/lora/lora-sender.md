# LoRaSender

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ_X/LoRa/LoRaSender`

## Objetivo

Envia pacotes LoRa de forma simples. A função de transmissão aguarda o rádio terminar o envio antes de continuar.

## Funcionamento

O rádio é configurado e, a cada intervalo, o firmware monta uma mensagem com um contador. A chamada de transmissão é bloqueante: o código só segue quando o envio termina ou ocorre erro.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Além dos parâmetros LoRa, ajuste os GPIOs do SX1262. Confira `SX1262_TCXO_VOLTAGE` e a configuração do RF switch conforme o módulo utilizado.

## Como testar

Use outro dispositivo com `LoRaReceiver` ou `LoRaReceiverCallback`, mantendo os mesmos parâmetros de rádio.

## Resultado esperado

Devem aparecer mensagens de envio e confirmação de `TxDone`/transmissão concluída.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

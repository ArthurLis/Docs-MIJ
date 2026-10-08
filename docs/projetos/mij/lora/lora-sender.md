# LoRaSender

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ/LoRa/LoRaSender`

## Objetivo

Envia pacotes LoRa de forma simples. A função de transmissão aguarda o rádio terminar o envio antes de continuar.

## Funcionamento

O rádio é configurado e, a cada intervalo, o firmware monta uma mensagem com um contador. A chamada de transmissão é bloqueante: o código só segue quando o envio termina ou ocorre erro.

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

Use outro dispositivo com `LoRaReceiver` ou `LoRaReceiverCallback`, mantendo os mesmos parâmetros de rádio.

## Resultado esperado

Devem aparecer mensagens de envio e confirmação de `TxDone`/transmissão concluída.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

# LoRaReceiverCallback

> **Plataforma:** MIJ — ESP32 + SX1276  
> **Modo:** LoRa  
> **Projeto:** `examples/MIJ/LoRa/LoRaReceiverCallback`

## Objetivo

Recebe pacotes LoRa usando uma interrupção/callback para avisar a aplicação quando uma recepção termina.

## Funcionamento

O SX1276 fica em recepção contínua. O DIO0 sinaliza `RxDone`; a aplicação detecta o evento, lê o pacote e chama a rotina de tratamento. O processamento do pacote acontece fora da interrupção.

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

Grave `LoRaSender` em outro dispositivo e envie pacotes com os mesmos parâmetros LoRa.

## Resultado esperado

O monitor deve indicar o evento de recepção e exibir o pacote recebido.

## Observações

Sender e Receiver precisam utilizar os mesmos parâmetros de modulação.

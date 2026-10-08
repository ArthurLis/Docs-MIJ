# OTAAReceiver

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRaWAN Class A / OTAA  
> **Projeto:** `examples/MIJ_X/LoRaWAN/OTAAReceiver`

## Objetivo

Realiza Join OTAA e demonstra o recebimento de downlinks.

## Funcionamento

Depois de ativar a sessão OTAA, o exemplo envia um uplink vazio periodicamente. Esse uplink abre RX1/RX2, onde um downlink pendente pode ser recebido.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Preencha somente as credenciais do modo deste exemplo e confira região/sub-band. Os exemplos definem um DR inicial para os uplinks de aplicação e mantêm ADR habilitado.

## Como testar

Cadastre o dispositivo OTAA, preencha as credenciais, aguarde o Join, coloque um downlink na fila do servidor e observe o próximo ciclo RX1/RX2.

## Resultado esperado

Quando houver downlink, o monitor deve mostrar a janela utilizada e os dados recebidos.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha JoinEUI, DevEUI e AppKey antes do teste e não publique chaves reais. Em Class A, downlinks só são recebidos nas janelas abertas depois de um uplink. O estado da sessão e os contadores são persistidos em NVS.

# OTAASender

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRaWAN Class A / OTAA  
> **Projeto:** `examples/MIJ_X/LoRaWAN/OTAASender`

## Objetivo

Realiza ativação OTAA e envia uplinks periódicos.

## Funcionamento

O dispositivo carrega JoinEUI, DevEUI e AppKey, tenta realizar o Join e salva a sessão em NVS. Depois do Join, envia uplinks periódicos e abre RX1/RX2 após cada envio.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Preencha somente as credenciais do modo deste exemplo e confira região/sub-band. Os exemplos definem um DR inicial para os uplinks de aplicação e mantêm ADR habilitado.

## Como testar

Cadastre o dispositivo OTAA no servidor, preencha as credenciais, grave o firmware e acompanhe primeiro o Join e depois os uplinks.

## Resultado esperado

O monitor deve confirmar a ativação OTAA e, em seguida, mostrar os uplinks enviados.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha JoinEUI, DevEUI e AppKey antes do teste e não publique chaves reais. O estado da sessão e os contadores são persistidos em NVS.

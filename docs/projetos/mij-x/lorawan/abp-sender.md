# ABPSender

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRaWAN Class A / ABP  
> **Projeto:** `examples/MIJ_X/LoRaWAN/ABPSender`

## Objetivo

Ativa uma sessão ABP e envia uplinks periódicos.

## Funcionamento

O dispositivo carrega as credenciais ABP, restaura o estado salvo em NVS e ativa a sessão. Depois envia pacotes em intervalos regulares e abre RX1/RX2 após cada uplink.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Preencha somente as credenciais do modo deste exemplo e confira região/sub-band. Os exemplos definem um DR inicial para os uplinks de aplicação e mantêm ADR habilitado.

## Como testar

Cadastre o dispositivo no servidor de rede, preencha DevAddr, NwkSKey e AppSKey, grave o firmware e acompanhe os uplinks no servidor.

## Resultado esperado

O monitor deve indicar a ativação ABP e, a cada envio, mostrar FCnt, FPort, DR, frequência e potência.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha-as somente no ambiente de teste e não publique chaves reais. O estado da sessão e os contadores são persistidos em NVS.

# ABPReceiver

> **Plataforma:** MIJ_X — ESP32-C6 + SX1262  
> **Modo:** LoRaWAN Class A / ABP  
> **Projeto:** `examples/MIJ_X/LoRaWAN/ABPReceiver`

## Objetivo

Demonstra o recebimento de downlinks em uma sessão ABP.

## Funcionamento

Em Class A o dispositivo não fica ouvindo o tempo todo. Este exemplo envia um uplink vazio apenas para abrir as janelas RX1 e RX2 e verificar se existe um downlink pendente.

## Hardware

Os GPIOs do ESP32-C6 são **genéricos nos exemplos**. Ajuste `SCLK`, `MISO`, `MOSI`, `NSS`, `RST`, `DIO1` e `BUSY` em `main/config.h` para a placa utilizada. Também confira se o módulo usa TCXO ou XTAL.

## Configuração

Arquivo principal: `main/config.h`.

Preencha somente as credenciais do modo deste exemplo e confira região/sub-band. Os exemplos definem um DR inicial para os uplinks de aplicação e mantêm ADR habilitado.

## Como testar

Cadastre o dispositivo em ABP, preencha as credenciais, coloque um downlink na fila do servidor e execute o exemplo.

## Resultado esperado

Se houver downlink pendente, o monitor deve informar RX1 ou RX2, FPort, FCnt, RSSI, SNR e o payload em HEX.

## Observações

As credenciais do repositório ficam zeradas de propósito. Preencha-as somente no ambiente de teste e não publique chaves reais. Em Class A, downlinks só são recebidos nas janelas abertas depois de um uplink. O estado da sessão e os contadores são persistidos em NVS.

# Perguntas e respostas

## Sender e Receiver precisam usar a mesma configuração LoRa?

Sim. Em LoRa direto, frequência, bandwidth, spreading factor, coding rate e sync word precisam ser compatíveis entre os rádios.

## O LoRaWAN Receiver fica ouvindo o tempo todo?

Não nos exemplos Class A. O dispositivo envia um uplink e abre as janelas RX1 e RX2 logo depois. O downlink precisa chegar em uma dessas janelas.

## Preciso preencher ABP e OTAA ao mesmo tempo?

Não. Cada exemplo usa somente o método indicado no nome do projeto. `ABPSender` e `ABPReceiver` usam credenciais ABP; `OTAASender` e `OTAAReceiver` usam credenciais OTAA.

## Por que as credenciais estão zeradas?

Para que nenhuma chave real seja publicada no repositório. Preencha os valores apenas no ambiente de teste antes de compilar.

## Os GPIOs da MIJ_X já são os pinos reais da placa?

Não. Eles são exemplos genéricos e precisam ser alterados em `main/config.h` de acordo com o hardware utilizado.

## Onde altero os parâmetros do rádio?

No `config.h` de cada projeto. A documentação de cada exemplo informa o caminho correto.

# LoRa x LoRaWAN

## LoRa

LoRa é uma tecnologia de comunicação sem fio de longo alcance e baixo consumo. Em uma comunicação LoRa direta, dois rádios podem trocar pacotes sem depender de uma infraestrutura LoRaWAN.

Para que a comunicação funcione, Sender e Receiver precisam utilizar parâmetros compatíveis, como frequência, bandwidth, spreading factor, coding rate e sync word.

## LoRaWAN

LoRaWAN é um protocolo de rede construído sobre a modulação LoRa. Ele define regras para autenticação, segurança, canais, Data Rate, janelas de recepção e comunicação com uma infraestrutura de rede.

Os dispositivos podem ser ativados por métodos como **ABP** ou **OTAA**.

## Quando usar cada um

Use LoRa direto quando a aplicação precisa de uma comunicação simples entre rádios e você controla os dois lados do enlace.

Use LoRaWAN quando os dispositivos precisam participar de uma rede com gateway, servidor de rede, gerenciamento de sessões e possibilidade de uplinks e downlinks controlados pelo protocolo.

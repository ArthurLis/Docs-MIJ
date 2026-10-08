# Configuração LoRa

Os parâmetros principais ficam no `config.h` do exemplo. Sender e Receiver precisam utilizar parâmetros compatíveis para se comunicarem.

## Parâmetros

| Parâmetro | Significado |
| --- | --- |
| Frequência | Frequência de operação utilizada pelo rádio para transmitir e receber os pacotes. |
| Bandwidth | Largura de banda do sinal LoRa. Valores maiores permitem transmissões mais rápidas, enquanto valores menores podem favorecer a sensibilidade do receptor. |
| Spreading Factor | Define o fator de espalhamento do sinal. Valores maiores aumentam o tempo no ar e normalmente favorecem alcance e robustez. |
| Coding Rate | Define a quantidade de redundância adicionada aos dados para auxiliar na correção de erros. |
| Sync Word | Valor usado para diferenciar transmissões LoRa que utilizam parâmetros de rádio semelhantes. |
| Potência | Potência de transmissão, normalmente expressa em dBm. Influencia alcance e consumo. |
| Preâmbulo | Sequência enviada no início do pacote para ajudar o receptor a detectar e sincronizar a transmissão. |

> [!IMPORTANT]
> Se frequência, bandwidth, SF, coding rate ou sync word forem incompatíveis, a recepção pode não funcionar.

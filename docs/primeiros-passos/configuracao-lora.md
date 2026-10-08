# Configuração LoRa

Os parâmetros principais ficam em `config.h`.

## Parâmetros

| Parâmetro | Significado |
| --- | --- |
| Frequência | Frequência de operação utilizada pelo rádio para transmitir e receber os pacotes. |
| Bandwidth | Largura de banda do sinal LoRa. Valores maiores permitem transmissões mais rápidas, enquanto valores menores podem favorecer a sensibilidade do receptor. |
| Spreading Factor | Define o fator de espalhamento do sinal LoRa. Valores maiores aumentam o tempo no ar e normalmente favorecem alcance e robustez. |
| Coding Rate | Define a quantidade de redundância adicionada aos dados para auxiliar na correção de erros durante a transmissão. |
| Potência | Potência utilizada na transmissão, normalmente expressa em dBm. Influencia alcance, consumo de energia e intensidade do sinal transmitido. |


> [!IMPORTANT]
> Sender e Receiver precisam usar parâmetros compatíveis.

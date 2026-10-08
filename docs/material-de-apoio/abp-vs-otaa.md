# ABP x OTAA

## ABP

**ABP (Activation By Personalization)** utiliza parâmetros de sessão configurados previamente no dispositivo. Por isso, não existe uma troca de Join antes do início dos uplinks.

É simples para testes e ambientes controlados, mas exige mais cuidado com o gerenciamento das chaves e dos contadores de quadro.

## OTAA

**OTAA (Over-The-Air Activation)** realiza um processo de Join com a rede. Após um Join aceito, o dispositivo passa a utilizar a sessão criada para a comunicação.

É normalmente a opção preferida para implantações LoRaWAN porque facilita a criação e renovação de sessões sem gravar previamente todas as chaves de sessão no dispositivo.

## Resumo

| ABP | OTAA |
| --- | --- |
| Sessão configurada previamente | Sessão criada por Join |
| Inicialização mais simples | Exige Join com a rede |
| Útil em testes e cenários controlados | Recomendado para redes gerenciadas |

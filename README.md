<div align="center">

# JVTECH — LoRa e LoRaWAN com ESP-IDF

Exemplos e documentação para as plataformas **MIJ** e **MIJ_X**, com comunicação **LoRa** e **LoRaWAN**.

**ESP-IDF 5.3.1** · **SX1276** · **SX1262** · **ABP** · **OTAA** · **Class A**

</div>

---

## Comparação entre SX1276 e SX1262

A tabela abaixo compara **apenas os rádios Semtech**, independentemente das placas MIJ ou MIJ_X.

| Característica | SX1276 | SX1262 |
| --- | --- | --- |
| Geração LoRa IP | 1ª geração | 2ª geração |
| Faixa de frequência | 137 a 1020 MHz | 150 a 960 MHz |
| Potência máxima de TX | +20 dBm | +22 dBm |
| Link budget máximo | 168 dB | 170 dB |
| Corrente em recepção | ~11 mA | ~4,6 mA |
| Spreading Factor LoRa | SF6 a SF12 | SF5 a SF12 |
| Taxa LoRa máxima | até 40 kbps | até 62,5 kbps |
| Sensibilidade máxima | até -148 dBm | até -148 dBm |

Em resumo, o **SX1262** é uma geração mais nova, com menor consumo em recepção, maior potência de transmissão, suporte a SF5 e LR-FHSS. O **SX1276**, por outro lado, possui uma faixa de frequência total mais ampla.

> Valores baseados na documentação oficial da Semtech.

---

## Sobre o repositório

Este repositório reúne os exemplos desenvolvidos para testes e estudo de comunicação LoRa e LoRaWAN nas plataformas JVTECH.

> [!IMPORTANT]
> As credenciais LoRaWAN deste repositório são placeholders zerados. Nunca publique chaves reais.

## Sumário

- [JVTECH — LoRa e LoRaWAN com ESP-IDF](#jvtech--lora-e-lorawan-com-esp-idf)
  - [Comparação entre SX1276 e SX1262](#comparação-entre-sx1276-e-sx1262)
  - [Sobre o repositório](#sobre-o-repositório)
  - [Sumário](#sumário)
  - [Plataformas](#plataformas)
  - [Documentação](#documentação)
    - [Informações gerais](#informações-gerais)
    - [Hardware](#hardware)
    - [Primeiros passos](#primeiros-passos)
    - [Material de apoio](#material-de-apoio)
  - [Exemplos](#exemplos)
    - [MIJ — ESP32 + SX1276](#mij--esp32--sx1276)
      - [LoRa](#lora)
      - [LoRaWAN](#lorawan)
    - [MIJ\_X — ESP32-C6 + SX1262](#mij_x--esp32-c6--sx1262)
      - [LoRa](#lora-1)
      - [LoRaWAN](#lorawan-1)
  - [Como começar](#como-começar)
    - [MIJ](#mij)
    - [MIJ\_X](#mij_x)
  - [Referências](#referências)
  - [Contatos](#contatos)

---

## Plataformas

| Plataforma | MCU | Rádio | Homologação | Código | Hardware |
| --- | --- | --- | --- | --- | --- |
| **MIJ** | ESP32 | SX1276 | Certificado Nº 24305-23-16470 | [Abrir projetos](examples/MIJ/) | [Ver documentação](docs/hardware/mij.md) |
| **MIJ_X** | ESP32-C6 | SX1262 | Certificado Nº 08444-25-16470 | [Abrir projetos](examples/MIJ_X/) | [Ver documentação](docs/hardware/mij-x.md) |

---

## Documentação

### Informações gerais

- [Características gerais](docs/caracteristicas-gerais.md)
- [Ambiente de desenvolvimento](docs/ambiente-de-desenvolvimento.md)
- [Visão geral dos projetos](docs/projetos/README.md)

### Hardware

- [Visão geral de hardware](docs/hardware/README.md)
- [MIJ — ESP32 + SX1276](docs/hardware/mij.md)
- [MIJ_X — ESP32-C6 + SX1262](docs/hardware/mij-x.md)

### Primeiros passos

- [Primeiros passos](docs/primeiros-passos/README.md)
- [Compilação e gravação](docs/primeiros-passos/compilacao-e-gravacao.md)
- [Configuração LoRa](docs/primeiros-passos/configuracao-lora.md)
- [Configuração LoRaWAN](docs/primeiros-passos/configuracao-lorawan.md)

### Material de apoio

- [Material de apoio](docs/material-de-apoio/README.md)
- [LoRa x LoRaWAN](docs/material-de-apoio/lora-vs-lorawan.md)
- [ABP x OTAA](docs/material-de-apoio/abp-vs-otaa.md)
- [Glossário](docs/material-de-apoio/glossario.md)

---

## Exemplos

Os exemplos estão separados por **plataforma** e por **tipo de comunicação**. Cada projeto é independente e pode ser aberto e compilado separadamente.

### MIJ — ESP32 + SX1276

[Visão geral da MIJ](docs/projetos/mij/README.md)

#### LoRa

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **LoRaSender** | [Abrir](examples/MIJ/LoRa/LoRaSender/) | [Ler](docs/projetos/mij/lora/lora-sender.md) |
| **LoRaReceiver** | [Abrir](examples/MIJ/LoRa/LoRaReceiver/) | [Ler](docs/projetos/mij/lora/lora-receiver.md) |
| **LoRaReceiverCallback** | [Abrir](examples/MIJ/LoRa/LoRaReceiverCallback/) | [Ler](docs/projetos/mij/lora/lora-receiver-callback.md) |
| **LoRaSenderNonBlockingCallback** | [Abrir](examples/MIJ/LoRa/LoRaSenderNonBlockingCallback/) | [Ler](docs/projetos/mij/lora/lora-sender-nonblocking-callback.md) |

#### LoRaWAN

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **ABPSender** | [Abrir](examples/MIJ/LoRaWAN/ABPSender/) | [Ler](docs/projetos/mij/lorawan/abp-sender.md) |
| **ABPReceiver** | [Abrir](examples/MIJ/LoRaWAN/ABPReceiver/) | [Ler](docs/projetos/mij/lorawan/abp-receiver.md) |
| **OTAASender** | [Abrir](examples/MIJ/LoRaWAN/OTAASender/) | [Ler](docs/projetos/mij/lorawan/otaa-sender.md) |
| **OTAAReceiver** | [Abrir](examples/MIJ/LoRaWAN/OTAAReceiver/) | [Ler](docs/projetos/mij/lorawan/otaa-receiver.md) |

---

### MIJ_X — ESP32-C6 + SX1262

[Visão geral da MIJ_X](docs/projetos/mij-x/README.md)

#### LoRa

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **LoRaSender** | [Abrir](examples/MIJ_X/LoRa/LoRaSender/) | [Ler](docs/projetos/mij-x/lora/lora-sender.md) |
| **LoRaReceiver** | [Abrir](examples/MIJ_X/LoRa/LoRaReceiver/) | [Ler](docs/projetos/mij-x/lora/lora-receiver.md) |
| **LoRaReceiverCallback** | [Abrir](examples/MIJ_X/LoRa/LoRaReceiverCallback/) | [Ler](docs/projetos/mij-x/lora/lora-receiver-callback.md) |
| **LoRaSenderNonBlockingCallback** | [Abrir](examples/MIJ_X/LoRa/LoRaSenderNonBlockingCallback/) | [Ler](docs/projetos/mij-x/lora/lora-sender-nonblocking-callback.md) |

#### LoRaWAN

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **ABPSender** | [Abrir](examples/MIJ_X/LoRaWAN/ABPSender/) | [Ler](docs/projetos/mij-x/lorawan/abp-sender.md) |
| **ABPReceiver** | [Abrir](examples/MIJ_X/LoRaWAN/ABPReceiver/) | [Ler](docs/projetos/mij-x/lorawan/abp-receiver.md) |
| **OTAASender** | [Abrir](examples/MIJ_X/LoRaWAN/OTAASender/) | [Ler](docs/projetos/mij-x/lorawan/otaa-sender.md) |
| **OTAAReceiver** | [Abrir](examples/MIJ_X/LoRaWAN/OTAAReceiver/) | [Ler](docs/projetos/mij-x/lorawan/otaa-receiver.md) |

---

## Como começar

1. Escolha a plataforma: **MIJ** ou **MIJ_X**.
2. Escolha um exemplo dentro de `LoRa/` ou `LoRaWAN/`.
3. Confira e ajuste o arquivo `config.h` do projeto.
4. Abra o terminal na pasta do exemplo.
5. Configure o target correto, compile, grave e abra o monitor.

### MIJ

```bash
idf.py set-target esp32
idf.py build
idf.py flash monitor
```

### MIJ_X

```bash
idf.py set-target esp32c6
idf.py build
idf.py flash monitor
```

Para mais detalhes, consulte [Compilação e gravação](docs/primeiros-passos/compilacao-e-gravacao.md).

---

## Referências

A lista de documentos, bibliotecas e materiais utilizados fica em [Referências](docs/referencias.md).

---

## Contatos

- **Telefone:** (41) 99269-6439
- **WhatsApp:** [(41) 99269-6439](https://wa.me/5541992696439)
- **E-mail:** [contato@jvtech.net.br](mailto:contato@jvtech.net.br)

Mais informações em [Contatos](docs/contatos.md).


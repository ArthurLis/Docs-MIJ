<div align="center">

# JVTECH — LoRa e LoRaWAN com ESP-IDF

Exemplos e documentação para as plataformas **MIJ** e **MIJ_X**, com comunicação **LoRa** e **LoRaWAN**.

**ESP-IDF 5.3.1** · **SX1276** · **SX1262** · **ABP** · **OTAA** · **Class A**

</div>

---

## Sobre o repositório

Este repositório reúne os exemplos desenvolvidos para testes e estudo de comunicação LoRa e LoRaWAN nas plataformas JVTECH.


## Sumário

- [JVTECH — LoRa e LoRaWAN com ESP-IDF](#jvtech--lora-e-lorawan-com-esp-idf)
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

| Plataforma | MCU | Rádio | Código | Hardware |
| --- | --- | --- | --- | --- |
| **MIJ** | ESP32 | SX1276 | [Abrir projetos](examples/MIJ/) | [Ver documentação](docs/hardware/mij.md) |
| **MIJ_X** | ESP32-C6 | SX1262 | [Abrir projetos](examples/MIJ_X/) | [Ver documentação](docs/hardware/mij-x.md) |

<!-- Adicionar fotos das placas, versões de hardware ou outras observações. -->

---

# Documentação

## Informações gerais

- [Características gerais](docs/caracteristicas-gerais.md)
- [Ambiente de desenvolvimento](docs/ambiente-de-desenvolvimento.md)
- [Visão geral dos projetos](docs/projetos/README.md)

## Hardware

- [Visão geral de hardware](docs/hardware/README.md)
- [MIJ — ESP32 + SX1276](docs/hardware/mij.md)
- [MIJ_X — ESP32-C6 + SX1262](docs/hardware/mij-x.md)

## Primeiros passos

- [Primeiros passos](docs/primeiros-passos/README.md)
- [Compilação e gravação](docs/primeiros-passos/compilacao-e-gravacao.md)
- [Configuração LoRa](docs/primeiros-passos/configuracao-lora.md)
- [Configuração LoRaWAN](docs/primeiros-passos/configuracao-lorawan.md)

## Material de apoio

- [Material de apoio](docs/material-de-apoio/README.md)
- [LoRa x LoRaWAN](docs/material-de-apoio/lora-vs-lorawan.md)
- [ABP x OTAA](docs/material-de-apoio/abp-vs-otaa.md)
- [Glossário](docs/material-de-apoio/glossario.md)

---

# Exemplos

Os exemplos estão separados por **plataforma** e por **tipo de comunicação**. Cada projeto é independente e pode ser aberto e compilado separadamente.

## MIJ — ESP32 + SX1276

[Visão geral da MIJ](docs/projetos/mij/README.md)

### LoRa

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **LoRaSender** | [Abrir](examples/MIJ/LoRa/LoRaSender/) | [Ler](docs/projetos/mij/lora/lora-sender.md) |
| **LoRaReceiver** | [Abrir](examples/MIJ/LoRa/LoRaReceiver/) | [Ler](docs/projetos/mij/lora/lora-receiver.md) |
| **LoRaReceiverCallback** | [Abrir](examples/MIJ/LoRa/LoRaReceiverCallback/) | [Ler](docs/projetos/mij/lora/lora-receiver-callback.md) |
| **LoRaSenderNonBlockingCallback** | [Abrir](examples/MIJ/LoRa/LoRaSenderNonBlockingCallback/) | [Ler](docs/projetos/mij/lora/lora-sender-nonblocking-callback.md) |

### LoRaWAN

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **ABPSender** | [Abrir](examples/MIJ/LoRaWAN/ABPSender/) | [Ler](docs/projetos/mij/lorawan/abp-sender.md) |
| **ABPReceiver** | [Abrir](examples/MIJ/LoRaWAN/ABPReceiver/) | [Ler](docs/projetos/mij/lorawan/abp-receiver.md) |
| **OTAASender** | [Abrir](examples/MIJ/LoRaWAN/OTAASender/) | [Ler](docs/projetos/mij/lorawan/otaa-sender.md) |
| **OTAAReceiver** | [Abrir](examples/MIJ/LoRaWAN/OTAAReceiver/) | [Ler](docs/projetos/mij/lorawan/otaa-receiver.md) |

---

## MIJ_X — ESP32-C6 + SX1262

[Visão geral da MIJ_X](docs/projetos/mij-x/README.md)

### LoRa

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **LoRaSender** | [Abrir](examples/MIJ_X/LoRa/LoRaSender/) | [Ler](docs/projetos/mij-x/lora/lora-sender.md) |
| **LoRaReceiver** | [Abrir](examples/MIJ_X/LoRa/LoRaReceiver/) | [Ler](docs/projetos/mij-x/lora/lora-receiver.md) |
| **LoRaReceiverCallback** | [Abrir](examples/MIJ_X/LoRa/LoRaReceiverCallback/) | [Ler](docs/projetos/mij-x/lora/lora-receiver-callback.md) |
| **LoRaSenderNonBlockingCallback** | [Abrir](examples/MIJ_X/LoRa/LoRaSenderNonBlockingCallback/) | [Ler](docs/projetos/mij-x/lora/lora-sender-nonblocking-callback.md) |

### LoRaWAN

| Exemplo | Código | Documentação |
| --- | --- | --- |
| **ABPSender** | [Abrir](examples/MIJ_X/LoRaWAN/ABPSender/) | [Ler](docs/projetos/mij-x/lorawan/abp-sender.md) |
| **ABPReceiver** | [Abrir](examples/MIJ_X/LoRaWAN/ABPReceiver/) | [Ler](docs/projetos/mij-x/lorawan/abp-receiver.md) |
| **OTAASender** | [Abrir](examples/MIJ_X/LoRaWAN/OTAASender/) | [Ler](docs/projetos/mij-x/lorawan/otaa-sender.md) |
| **OTAAReceiver** | [Abrir](examples/MIJ_X/LoRaWAN/OTAAReceiver/) | [Ler](docs/projetos/mij-x/lorawan/otaa-receiver.md) |

---

# Como começar

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

# Referências

A lista de documentos, bibliotecas e materiais utilizados fica em [Referências](docs/referencias.md).

---

# Contatos

Os canais de contato e informações institucionais podem ser adicionados em [Contatos](docs/contatos.md).

<!-- Espaço para informações adicionais da JVTECH. -->

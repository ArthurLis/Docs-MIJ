# Compilação e gravação

## Seleção do target

Para MIJ:

```bash
idf.py set-target esp32
```

Para MIJ_X:

```bash
idf.py set-target esp32c6
```

## Build

```bash
idf.py build
```

## Flash e monitor

```bash
idf.py -p PORTA flash monitor
```

<!-- Adicionar observações de porta, driver e troubleshooting. -->

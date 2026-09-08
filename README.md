# Controle Térmico com Arduino — Atuador e Alarme por Temperatura

> Projeto original: **Fantabulous Elzing** (Tinkercad) • Placa: Arduino UNO R3

Sistema embarcado simples que monitora a temperatura com um sensor TMP36 e aciona dois atuadores de forma escalonada: um motor DC como atuador de resfriamento/ventilação e um buzzer piezoelétrico como alarme sonoro de superaquecimento.

## 📸 Montagem

![Esquema de Montagem](Fantabulous%20Elzing.png)

*Arquivo original: `Fantabulous Elzing.png` (1366×554) — montagem no Tinkercad com Arduino UNO.*

## 📋 Descrição

O firmware em `fantabulous_elzing1.ino:18-33` lê o sensor analógico a cada ~2s, converte para °C e aplica duas regras:

| Condição | Ação | Trecho |
|---|---|---|
| `temp >= 30°C` | Liga motor em `D2` (`HIGH`) | `fantabulous_elzing1.ino:22-26` |
| `temp < 30°C` | Desliga motor (`LOW`) | `fantabulous_elzing1.ino:24-26` |
| `temp >= 50°C` | Toca buzzer em `D4` a 523 Hz (Dó5) por 1s | `fantabulous_elzing1.ino:28-30` |
| `temp < 50°C` | Silencia buzzer | `fantabulous_elzing1.ino:31` |

A conversão usada é `fantabulous_elzing1.ino:21`:

```cpp
temp = (-40 + 0.488155 * (analogRead(A0) - 20));
```

> Baseada na curva do TMP36 (10 mV/°C, offset 500 mV a 0°C). O fator `0.488155` ≈ `500/1023`.

## 🧩 Componentes

Extraído de `Fantabulous Elzing.brd` (Eagle 9.7.0, biblioteca `Tinkercad`):

| Qtd | Componente | Ref. no `.brd` | Especificação |
|-----|------------|----------------|---------------|
| 1 | Arduino UNO R3 | `UUNO` | `ARDUINO-UNO-R3-SHIELD` |
| 1 | Sensor de Temperatura | `UTEMP` | `TMP36GT9Z_TO92` (TO-92) |
| 1 | Buzzer Piezoelétrico | `PIEZOBUZZER` | `PIEZO` |
| 1 | Motor DC | `MMOTOR` | `MOTOR_HDR` |
| 1 | Resistor | `R2` | `0.3043kΩ` (≈ 304 Ω, `RESAD1016W63L850D250B`) |

## 🔌 Esquema de Ligações

Sinais definidos em `Fantabulous Elzing.brd`:

| Sinal | Conexão |
|-------|---------|
| `UUNO_GND` | `UTEMP.1` ↔ `UUNO.GND` (x3) ↔ `PIEZOBUZZER.2` ↔ `MMOTOR.-` |
| `UUNO_5V` | `UTEMP.3` ↔ `UUNO.5V` |
| `UUNO_A0` | `UTEMP.2` ↔ `UUNO.A0` |
| `UUNO_D4` | `PIEZOBUZZER.1` ↔ `UUNO.D4` |
| `UUNO_D2` | `UUNO.D2` ↔ `R2.1` ↔ `R2.2` ↔ `MMOTOR.+` |
| `MMOTOR_+` | Via `R2` (ver acima) |

Resumo prático:

* **TMP36:** pino 1 → GND, pino 2 → A0, pino 3 → 5V
* **Buzzer:** terminal 1 → D4, terminal 2 → GND
* **Motor:** `+` → D2 via resistor 304 Ω, `-` → GND

> **Nota:** A imagem `Fantabulous Elzing.png` mostra a fiação de forma ilustrativa; a referência fiel para reprodução é o `.brd`.

## ⚙️ Como Funciona

```
loop() a cada 2s
  ├─ delay(1000)                     // fantabulous_elzing1.ino:20
  ├─ temp = -40 + 0.488155*(A0-20)   // fantabulous_elzing1.ino:21
  ├─ if temp >=30 → D2=HIGH else LOW // fantabulous_elzing1.ino:22-26
  ├─ delay(1000)                     // fantabulous_elzing1.ino:27
  └─ if temp >=50 → tone(D4,523) else noTone // fantabulous_elzing1.ino:28-32
```

Estados:

* **< 30°C:** sistema em repouso.
* **30°C – 49°C:** motor ligado, buzzer desligado.
* **≥ 50°C:** motor + buzzer ligados (alarme).

## 💻 Código

Configuração em `fantabulous_elzing1.ino:12-16`:

```cpp
void setup() {
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT); // Motor
  pinMode(4, OUTPUT); // Buzzer
}
```

Variáveis `i`, `motor` e `buzzer` declaradas em `fantabulous_elzing1.ino:5-9` não são utilizadas — podem ser removidas.

## 🛠️ Como Montar e Usar

### 1. Montagem

1. Conecte o TMP36 conforme tabela acima.
2. Conecte o buzzer ao pino D4 e GND (atente à polaridade `+`/`-`).
3. Conecte o motor via resistor 304 Ω entre D2 e `+` do motor; `-` do motor ao GND.

### 2. Upload

1. Abra `fantabulous_elzing1.ino` na Arduino IDE.
2. Selecione **Placa: Arduino Uno** e a porta correta.
3. Clique em **Carregar**.

### 3. Teste

* Aproxime uma fonte de calor do TMP36 (ou aqueça com as mãos).
* Observe o motor acionar a partir de ~30°C e o buzzer a partir de ~50°C.
* Use o Monitor Serial (adicione `Serial.begin(9600)` e `Serial.println(temp)` se quiser debug).

## 📁 Estrutura do Projeto

```
circuitoEletrônico1/
├── fantabulous_elzing1.ino      # Firmware Arduino
├── Fantabulous Elzing.brd       # Esquemático/board Eagle 9.7.0
├── Fantabulous Elzing.png       # Render da montagem
└── README.md
```

## 🚀 Melhorias Sugeridas

* **Acionamento do motor:** hoje o motor é ligado direto ao pino D2 via resistor — um pino digital não fornece corrente suficiente e pode danificar a placa. Use transistor NPN/MOSFET + resistor de base + diodo flyback.
* **Histerese:** adicionar banda (ex.: liga a 30°C, desliga a 28°C) para evitar chaveamento rápido.
* **Não-bloqueante:** substituir `delay(1000)` por `millis()` para leitura mais responsiva.
* **Calibração:** trocar fórmula manual por `tempC = (analogRead(A0) * 5.0/1023 - 0.5) * 100`.
* **Feedback:** adicionar LED indicador e `Serial` para telemetria.

## 📄 Licença

Projeto educacional — uso livre para estudo. Sem licença específica definida.

---

*Gerado para o desafio DIO • IoT com Arduino*

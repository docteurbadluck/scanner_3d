Ll# KiCad Components — DC Motor Circuit

## Composants et bibliothèques

### Raspberry Pi Pico
- **Bibliothèque** : `MCU_RaspberryPi_RP2040`
- **Symbole** : `RaspberryPi_Pico`
- **Note** : disponible nativement dans KiCad 6+

---

### IBT-2 (BTS7960 H-Bridge module)
- **Bibliothèque** : absent des bibliothèques standard — créer un connecteur générique
- **Symbole à utiliser** : `Connector_Generic:Conn_01x06` pour la partie contrôle + `Connector_Generic:Conn_01x04` pour la partie puissance
- **Pins contrôle (Conn_01x06)** :
  1. RPWM ← GP6 Pico
  2. LPWM ← GP7 Pico
  3. R_EN ← 3.3V
  4. L_EN ← 3.3V
  5. VCC  ← 3.3V ou 5V
  6. GND  ← GND commun
- **Pins puissance (Conn_01x04)** :
  1. B+  ← 24V PSU
  2. B-  ← GND PSU
  3. M+  → Moteur fil 1
  4. M-  → Moteur fil 2

---

### Moteur DC (JGB37)
- **Bibliothèque** : `Device`
- **Symbole** : `Motor_DC`

---

### Résistance shunt servo (0.5Ω)
- **Bibliothèque** : `Device`
- **Symbole** : `R`
- **Valeur** : `0.5Ω` (ou `510mΩ`) — puissance min 0.5W
- **Câblage** : en série sur la ligne VCC 5V → servo ; point milieu → GP26 (ADC0)
- **Note** : le Pico lit la tension aux bornes (V = I × 0.5Ω) via son ADC 12-bit (3.3V ref)

---

### Boutons endstop (×2)
- **Bibliothèque** : `Device`
- **Symbole** : `SW_Push`
- **Câblage** : une borne → GPIO (GP8 ou GP9), autre borne → GND

---

### Connecteur alimentation 24V
- **Bibliothèque** : `Connector_Generic`
- **Symbole** : `Conn_01x02`
- **Pins** : 24V + GND

---

### Symboles de puissance
- **Bibliothèque** : `power`
- **Symboles** :
  - `GND` — masse commune
  - `+3V3` — alimentation logique Pico
  - `+24V` — alimentation moteur (à créer si absent : Add Power Port)

---

## Conseils KiCad

- Pour créer `+24V` : **Place → Add Power Port** → nommer `+24V`
- L'IBT-2 n'existant pas en bibliothèque standard, utilise deux connecteurs génériques renommés avec des **net labels** (`RPWM`, `LPWM`, `M+`, `M-`, etc.)
- Les pull-up des endstops sont **internes au Pico** — pas de résistance à placer dans le schéma
- Les diodes flyback sont **intégrées au BTS7960** — pas à placer dans le schéma

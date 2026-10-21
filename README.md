# 🔴 Iron Man Reactor — Relay Edition

Arduino sketch that drives an **arc reactor lamp** through a relay, with a
"power-up" flicker sequence and simple one-button control — just like the
reactor spinning up in the movies.

Built for the **Keyestudio KS0077 / KS0078 / KS0079** boards with a 5V relay
module and an **HT-5W** lamp.

## ✨ Features

- 🔥 **Cinematic startup** — a sequence of short flashes that accelerate into
  a steady glow, simulating the reactor powering on.
- 🔘 **Single button control** — no extra wiring beyond a push button.
  - **Single press** → play the startup animation, then stay lit.
  - **Press while lit** → turn the reactor off instantly.
  - **Double press** → skip the animation and snap straight to full power.
- ⚙️ **Configurable** — relay polarity and timing are plain constants at the
  top of the sketch.

## 🔧 Hardware

| Part | Notes |
|---|---|
| Keyestudio KS0077 / KS0078 / KS0079 (or any Arduino-compatible board) | Microcontroller |
| 5V relay module | Switches the lamp |
| HT-5W lamp | The "reactor" light source |
| Push button | Momentary, normally open |
| External power supply | Matched to the HT-5W's voltage/current rating |

## 🔌 Wiring

**Relay module:**

| Relay pin | Connects to |
|---|---|
| `S` / `IN` | D9 |
| `+` / `VCC` | 5V |
| `-` / `GND` | GND |

**Button:**

| Pin | Connects to |
|---|---|
| D2 | Button → GND (uses `INPUT_PULLUP`, no external resistor needed) |

**Relay contacts (switched circuit):**

```
Power supply (+) ── COM
Relay NO         ── HT-5W (+)
HT-5W (-)        ── Power supply (-)
```

> ⚠️ **Important:** Power the HT-5W from a supply matching its required
> voltage — **never** from the Arduino's D9 pin directly.

## 🚀 Getting started

1. Wire everything up as described above.
2. Open [`ironman-reactor-relay.ino`](ironman-reactor-relay.ino) in the
   [Arduino IDE](https://www.arduino.cc/en/software).
3. Select your board and port, then upload the sketch.
4. Press the button and watch the reactor power up. ⚡

## ⚙️ Configuration

A few constants at the top of the sketch let you tune the behavior:

```cpp
const byte RELAY_PIN  = 9;   // Relay signal pin
const byte BUTTON_PIN = 2;   // Button input pin
const bool RELAY_ACTIVE_HIGH = true; // Set false if your relay triggers in reverse
const unsigned long DOUBLE_PRESS_WINDOW_MS = 400; // Double-press detection window
```

The flicker pattern itself is defined in the `offTimes[]` array inside
`reactorStart()` — adjust the values to speed up, slow down, or restyle the
power-up animation.

## 📄 License

Released under the [MIT License](LICENSE).

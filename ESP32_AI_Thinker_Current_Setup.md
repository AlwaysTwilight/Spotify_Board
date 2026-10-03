# ESP32 + AI-Thinker VC-02 + TFT Current Setup

## Purpose

This document describes the **current physical setup** of the project so an AI coding agent (Codex) can understand the hardware before writing or changing code.

The user is building an ESP32 project that will eventually use:

- ESP32 DevKit-style board
- TFT display
- AI-Thinker VC-02 voice recognition module
- Microphone
- Speaker
- MB102 breadboard power supply
- A touch/gesture sensor may be added later

The immediate goal is to get the ESP32 communicating with the AI-Thinker VC-02, then integrate voice commands with the TFT.

---

# 1. ESP32

## Board

The ESP32 is a normal ESP32 DevKit-style board.

In PlatformIO, the project is being created with:

- Board: `Espressif ESP32 Dev Module`
- Framework: `Arduino`

The board silkscreen uses `D` labels for the GPIO pins.

Relevant UART pins currently planned/connected:

- `D16` = ESP32 UART RX
- `D17` = ESP32 UART TX

These are being used to communicate with the VC-02.

---

# 2. AI-Thinker VC-02

The module is an **AI-Thinker VC-02 / VC-02-Kit**.

The metal shield is marked approximately:

- VC-02
- AI-Thinker
- Offline Voice
- Command Entry: 150
- Flash: 2MB

The development board exposes these pins.

## Left-side pins on the VC-02-Kit

From top to bottom:

- SDA_5V
- SCL_5V
- SDA
- SCL
- IOA27
- IOB8
- RX1
- TX1
- GND
- VCC

## Right-side pins

- VCC
- NC
- TCK
- TMS
- GND
- DAC_L
- DAC_R
- 3V3OUT
- GND

For the current project, only these VC-02 pins are being used:

- VCC
- GND
- TX1
- RX1

Everything else is currently left unconnected unless specifically required later.

---

# 3. Current VC-02 ↔ ESP32 wiring

The current intended UART wiring is:

| AI-Thinker VC-02 | ESP32 |
|---|---|
| TX1 | D16 |
| RX1 | D17 |

Important:

- TX and RX are crossed.
- VC-02 TX1 goes to ESP32 D16.
- VC-02 RX1 goes to ESP32 D17.

Do not change these connections without checking the physical board first.

---

# 4. VC-02 power

The VC-02 is powered from the MB102 5V rail.

Current connection:

| VC-02 | MB102 / breadboard |
|---|---|
| VCC | +5V rail |
| GND | GND / negative rail |

The VC-02 should NOT be powered from the ESP32 3.3V pin.

The VC-02 `3V3OUT` pin is currently NOT being used.

---

# 5. MB102 power supply

The project uses an MB102 breadboard power-supply module.

The MB102 has separate voltage-selection jumpers for the two sides of the breadboard.

Each selector has **4 pins**, labelled conceptually:

- A-B = 5V
- B-C = OFF
- C-D = 3.3V

A yellow jumper cap connects two adjacent pins.

## Current state

The jumper on the side being used for the VC-02 power rail has been moved to:

**A-B = 5V**

Therefore that breadboard positive rail is being used as the VC-02's 5V supply.

There is another voltage selector on the other side. It should not be changed unless necessary because the TFT/other existing circuit may use its existing voltage.

The MB102 ground/negative rail is shared with the ESP32 circuit.

---

# 6. TFT display

There is a red TFT display already connected to the ESP32.

The TFT has these labels:

- LED
- SCK
- SDA
- A0
- RESET
- CS
- GND
- VCC

The user considers the TFT + ESP32 setup already established and does NOT want it unnecessarily rewired.

Previously discussed/used TFT wiring included:

- TFT SCK → ESP32 D18
- TFT SDA → ESP32 D23
- TFT RESET → ESP32 D2
- TFT CS → ESP32 D5
- TFT power is 3.3V
- TFT GND is common ground
- TFT LED is powered appropriately from the 3.3V side

### Important

There has been some previous uncertainty about the exact TFT `A0` pin mapping. Do NOT blindly rewrite or change the TFT wiring. If code requires the TFT pin definitions, first inspect the user's current wiring/photo or ask the user to confirm the A0 connection.

The TFT currently works/has been wired separately and should be preserved.

---

# 7. Microphone

A small microphone is connected to the AI-Thinker VC-02-Kit using its existing small connector.

The exact microphone pinout has NOT been independently verified.

Do not invent a microphone wiring diagram.

For the current setup, assume the microphone is physically connected to the VC-02 as shown by the user.

---

# 8. Speaker

A small rectangular speaker with red/black wires is connected to the VC-02-Kit.

The exact speaker wiring/pinout has NOT been independently verified.

Do not change the speaker wiring unless required and confirmed.

---

# 9. Current physical architecture

The intended physical architecture is:

```text
                    USB
                     |
                     v
                 +-------+
                 | ESP32 |
                 +-------+
                  |     |
             D16  |     | D17
                  |     |
                 TX1   RX1
                  |     |
                  v     v
              +-----------+
              |  VC-02    |
              | AI-Thinker|
              +-----------+
                |       |
              Mic     Speaker

ESP32
  |
  +---- TFT display
  |
  +---- common GND

MB102
  |
  +---- 5V rail ----> VC-02 VCC
  |
  +---- GND rail ---> VC-02 GND
                         |
                         +---- common ground with ESP32
```

---

# 10. Software status

At the moment, the hardware has been physically connected, but the software has NOT yet been configured.

There is currently no custom ESP32 firmware for the VC-02 communication.

The user is using:

**VS Code + PlatformIO**

The PlatformIO project is being created with:

- Project name: `AI Thinker`
- Board: `Espressif ESP32 Dev Module`
- Framework: `Arduino`

No VC-02 library or communication code should be assumed to already exist.

---

# 11. What Codex should do next

The next software goal is to create a minimal test program that:

1. Starts the ESP32.
2. Starts a hardware UART on the pins:
   - RX = D16
   - TX = D17
3. Uses the correct VC-02 UART configuration.
4. Reads data coming from the VC-02.
5. Prints received data to the USB Serial Monitor.
6. Does NOT modify the TFT code yet.
7. Does NOT add touch-sensor code yet.
8. Does NOT assume the user has trained/configured custom voice commands.
9. Provides simple diagnostic output so we can determine whether the ESP32 and VC-02 are communicating.

Only after UART communication is confirmed should the project move to:

- VC-02 voice-command handling
- TFT display reactions
- speaker/audio behavior
- Wi-Fi features
- touch/gesture sensor integration

---

# 12. Important instructions for Codex

## Do not

- Change the physical pin wiring without asking.
- Change the TFT pins without asking.
- Power the VC-02 from 3.3V.
- Connect VC-02 `3V3OUT` as its power input.
- Assume the microphone or speaker pinout without verification.
- Add libraries unnecessarily.
- Rewrite the whole project before testing basic UART communication.
- Assume custom voice commands have already been configured.

## Do

- Keep the first firmware test extremely small.
- Use ESP32 hardware UART.
- Use D16/D17 as the current VC-02 communication pins.
- Keep the TFT untouched during the first VC-02 communication test.
- Print useful diagnostic information to the USB serial monitor.
- Explain any required library or configuration before adding it.

---

# 13. Current hardware checklist

- [x] ESP32 connected to breadboard
- [x] TFT connected to ESP32
- [x] MB102 connected to breadboard
- [x] MB102 selected to 5V on the rail used for VC-02
- [x] VC-02 VCC connected to 5V
- [x] VC-02 GND connected to common ground
- [x] VC-02 TX1 connected to ESP32 D16
- [x] VC-02 RX1 connected to ESP32 D17
- [x] Microphone physically connected to VC-02
- [x] Speaker physically connected to VC-02
- [x] VS Code installed
- [x] PlatformIO installed
- [x] PlatformIO project being created as Espressif ESP32 Dev Module + Arduino
- [ ] VC-02 UART software test
- [ ] Confirm VC-02 communication
- [ ] Integrate voice commands with TFT
- [ ] Add touch/gesture sensor later

---

# 14. Current priority

**Priority #1: Get reliable ESP32 ↔ VC-02 UART communication working.**

Do not proceed to complex application logic until the basic UART communication has been tested successfully.

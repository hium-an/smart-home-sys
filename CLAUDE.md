# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Embedded C smart home system using **3 × AT89C52** microcontrollers (8051 family, 8 KB Flash, 256 B RAM, 11.0592 MHz crystal). Academic group project (HKII semester, Group One).

- **MCU1 (Security):** LCD + password login + GPIO signals → MCU2
- **MCU2 (Door/Sound):** DC motor + buzzer/speaker + UART log. Receives GPIO from MCU1.
- **MCU3 (Environ):** 7-SEG display + auto fan + BULB PWM lighting

**Toolchain:**
- **Keil uVision** — IDE and Keil C51 compiler. 3 separate projects in `keil/security/`, `keil/door/`, `keil/environ/`.
- **Proteus** — circuit simulation. Simulation files live in `sim/`.

## Build & Flash

Building is done through Keil uVision GUI — there is no CLI build script.
There are **3 separate Keil projects** (one per MCU):

| Project | Folder | Define | HEX output |
|---------|--------|--------|------------|
| Security | `keil/security/` | `PROJ_MCU1` | `Security.hex` |
| Door | `keil/door/` | `PROJ_MCU2` | `Door.hex` |
| Environ | `keil/environ/` | `PROJ_MCU3` | `Environ.hex` |

1. Open the `.uvproj` file inside the relevant `keil/` subfolder.
2. **Build:** `Project → Build Target` (or `F7`).
3. **Rebuild all:** `Project → Rebuild All Target Files`.

To run the simulation, open the `.DSN` file inside `sim/` with Proteus and load each HEX into its corresponding AT89C52 chip.

## Repository Layout

```
src/        C source files (application logic, drivers)
inc/        C header files
asm/        ARM assembly routines (e.g., precise timing delays)
keil/       Keil uVision project files (.uvprojx, scatter files)
sim/        Proteus simulation schematics
build/      Compiler output — git-ignored
docs/       Project report and documentation
```

## Architecture

**MCU1 — Security (PROJ_MCU1):**
- **`main1.c`** — entry point: welcome screen, login loop, GPIO signal output.
- **`lcd.c` / `lcd.h`** — LCD 16×2 driver, 4-bit mode (data on P3.4–P3.7, RS=P2.6, E=P2.7).
- **`security.c` / `security.h`** — 4-digit password login, lockdown, change-password flow.

**MCU2 — Door/Sound (PROJ_MCU2):**
- **`main_door.c`** — entry point: polls GPIO signals from MCU1, controls door/sound.
- **`door.c` / `door.h`** — DC motor control via L293D + encoder counting on INT0.
- **`sound.c` / `sound.h`** — buzzer + speaker driver (welcome melody, beep, alarm).
- **`uart.c` / `uart.h`** — UART 9600 8N1 via Timer1; timestamped log helper.

**MCU3 — Environ (PROJ_MCU3):**
- **`main2.c`** — entry point: 7-SEG 2-digit temp, 7-SEG 4-digit brightness, auto fan, BULB PWM, UART log.
- **`sensor.c` / `sensor.h`** — button-simulated temperature, switch-based light sensor (P1.2).
- **`uart.c` / `uart.h`** — shared with MCU2, UART 9600 8N1 via Timer1.

**Shared:**
- **`inc/config.h`** — **single source of truth** for all pin definitions (3 MCU sections via `#ifdef`), timer reload values, and tunable constants.

**Inter-MCU communication (GPIO):**
- MCU1.P1.4 (`SIG_UNLOCK`) → MCU2.P1.1 (`SIG_UNLOCK_IN`): trigger door open.
- MCU1.P1.5 (`SIG_ALARM`) → MCU2.P1.0 (`SIG_ALARM_IN`): trigger alarm.
- MCU1.P1.6 (`SIG_WRONG`) → MCU2.P1.3 (`SIG_WRONG_IN`): wrong password beep.
- MCU1.P2.1 (`SIG_JAM`) → MCU2.P1.4 (`SIG_JAM_IN`): door jam signal.

Peripheral access uses direct SFR/sbit manipulation (Keil C51 extensions). No RTOS — simple super-loop with Timer0 ISR for 1 ms tick.

## Coding Conventions

- **C89/C51** standard (Keil C51 compiler; `--c99` is not available for 8051 target).
- All hardware-specific constants (pin definitions, timer reload values, thresholds) belong in `inc/config.h` — never scattered in `.c` files.
- Code comments inside functions: **tiếng Việt không dấu** (Vietnamese without diacritics).
- Docs and guide files: **tiếng Việt có dấu**.
- Assembly routines in `asm/` must have a matching `extern` C declaration and follow the 8051 register convention (args/return via A, R0–R7).

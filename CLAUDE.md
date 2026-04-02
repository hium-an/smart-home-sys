# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Embedded C smart home system targeting an ARM Cortex-M microcontroller. Academic group project (HKII semester, Group One).

**Toolchain:**
- **Keil uVision** — IDE and ARM compiler (ARMCC/ARMCLANG). Project files live in `keil/`.
- **Proteus** — circuit simulation. Simulation files live in `sim/`.

## Build & Flash

Building is done through Keil uVision GUI — there is no CLI build script:

1. Open the `.uvprojx` file inside `keil/` with Keil uVision.
2. **Build:** `Project → Build Target` (or `F7`).
3. **Rebuild all:** `Project → Rebuild All Target Files`.
4. **Flash to hardware:** `Flash → Download` (or `F8`).
5. Compiled artifacts (`.hex`, `.obj`, `.lst`, etc.) are written to `build/` and are git-ignored.

To run the simulation, open the `.DSN` file inside `sim/` with Proteus.

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

- **`main.c`** — application entry point, initialization, main loop / RTOS tasks.
- **`sensor.c` / `sensor.h`** — sensor abstraction layer (read ADC, parse data).
- **`asm/delay.asm`** — cycle-accurate delay routines called from C; used where `HAL_Delay` resolution is insufficient.

Peripheral access uses direct register manipulation or CMSIS/HAL macros — avoid platform-agnostic abstractions that assume an OS.

## Coding Conventions

- C99 standard (`--c99` flag in Keil).
- All hardware-specific constants (pin numbers, register addresses, timing values) belong in `inc/` headers, not scattered in `.c` files.
- Assembly functions must have a matching C declaration in a header and follow the ARM Procedure Call Standard (AAPCS).

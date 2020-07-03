# NES Emulator

![Windows](https://github.com/Lauchmelder23/NesEmulator/workflows/Windows/badge.svg?branch=development)
![Ubuntu](https://github.com/Lauchmelder23/NesEmulator/workflows/Ubuntu/badge.svg?branch=development)
![MacOS](https://github.com/Lauchmelder23/NesEmulator/workflows/MacOS/badge.svg?branch=development)

## Installation
Simply clone the repository and generate your build files with cmake. Then build and install via `sudo make install`.

On Linux and MacOS you need to manually install the SDL2 sdk via your package manager.

Before building the project you might want to have a look at `NES Emulator/editme.hpp` and see what features you want to enable/disable. (Printing the instruction slows down program execution by a ton, but it allows you to look at what the CPU is doing

## Controls
* C - Step forward one instruction
* F - Step forward one frame
* R - Reset the CPU
* I - Interrupt
* N - Non-maskable interrupt
* SPACE - Run continuously

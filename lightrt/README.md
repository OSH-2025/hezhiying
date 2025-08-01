# LightRT

**LightRT** is a lightweight operating system framework for STM32 F1 devices (ARM Cortex-M) by Hezhiying Team @ OSH course, 2025 spring semester at USTC. The system is written in a simplicity-first style.

## Construction

The entire system is written in C programming language and ARM assembly (for low level operations). There are several parts:

- `main.c`: Entry code and initialization
- `task.c` and `task.h`: Task management
- `uart.c` and `uart.h`: UART message print
- `svcentry.c`, `svc.h`, `user/syscall.c`, `user/syscall.h`: Syscall support; these files are automatically generated by `svcc` utility with `svc.def` definitions during build process.
- `svcimpl.c`: Implements the syscall routines. 
- `signal.c` + `signal.h`: Signaling service
- `rand.c` + `rand.h`: Task specific random generator
- `mm.c` + `mm.h`: Memory allocation manager
- `dma.c` + `dma.h`: DMA controller interface
- `errno.h`: Global error code definitions
- `lightrt.h`: Global system header file

The Renode simulator does not support all the peripherals so we implement some of them in `periph/`:

- `dma.cs`: Simulated STM32F1 DMA controller (note that this is different from the STM32DMA periph given in Renode. That is for F4 MCUs supporting stream mode, not channel mode in F1 MCUs)
- `extmem.cs`: Simulated external memory (shared memory) controller, use with `smserver` - the shared memory server over TCP.

## Building

Building the system is relatively easy. Just use GNU `make` and you will get the `firmware.bin` / `firmware.elf` binaries. For first time build you should `make` twice (see below).

The building process requires the `libopencm3` (installed at `libopencm3/`). Building of this library is included in the Makefile and will be done automatically.

Note: When you build for the first time the GCC would report undefined references (they are from automatically generated files but these files are not scanned by `make` after generation). Use `make` for the second time will fix this issue.

## Testing

TL;DR
- `make smsvc-start`
- `make start`
- `make uart`
(Note that these should be run parallel)

Renode is used to simulate the real running environment. You can use `make start` to start a Renode instance from `renode/` (requires Renode binary at `renode/renode`). Use `make gdb` to start a GDB multiarch for debugging and connect it to the Renode simulator.

To see the UART, please use `make uart` after the machine starts. To use the shared memory controller (virtual), use `make smsvc-start` before the machine starts.

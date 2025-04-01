# Background and Related Project Research

[中文](background_cn.md) | [English](background.md)

Embedded systems are widely used in various devices, most of which have only one processor core and typically run a single task. To run multiple tasks, the most primitive method is to use interrupts to interrupt the main thread's execution, thereby handling sudden events. Based on timer interrupts, there are a series of embedded OSs that can achieve pseudo (through scheduling) multitasking. We plan to design a computational system with multiple processors, where each processor can run multiple tasks, as well as the required OS software, to meet the needs of robots and other devices.

---

## 1. Bare-metal Programming (No Operating System)

- **Single-task Sequential Execution**:
    In simple systems without an operating system, the MCU executes code sequentially through a `while(1)` loop, essentially running a single task.
- **Pseudo-multitasking**:
    Simulating multitasking through interrupts and state machines. For example:
    1. **Interrupt Service Routine (ISR)**: Handles high-priority tasks (such as button detection, communication reception).
    2. **State Machine in Main Loop**: Time-slices processing of multiple tasks (such as display refresh, sensor data acquisition). **Still single-threaded**, but logically divided into multiple tasks.

---

## 2. Using Real-time Operating Systems (RTOS)

- **Multitasking Concurrency**:
    RTOSs (such as FreeRTOS, uC/OS, ThreadX) allow multiple tasks (threads) to be scheduled concurrently on a single-core MCU through **time slicing** or **priority preemption**.

---

## 3. Typical Scenario Examples

- **Without RTOS**: Logically divided into multiple functional modules but physically runs as a single thread.
- **With RTOS**: STM32F4 + FreeRTOS can stably run 5~20 tasks (depending on task resource requirements).
- **High-performance MCU**: STM32H7 dual-core can handle two high-load tasks in parallel, supplemented by multiple low-priority tasks.

---

## 4. Related Project Research

### [FreeRTOS](https://github.com/FreeRTOS)

FreeRTOS is a very popular open-source real-time operating system widely used in various embedded systems, including STM32 microcontrollers.

The project can directly use its scheduler and communication mechanisms. However, it lacks distributed support and requires manual expansion.

Features:

- Supports task management, semaphores, queues, and more.
- Optimized for resource-constrained devices.
- Provides extensive documentation and community support.
- STM32 Support: FreeRTOS provides support for STM32 and integrates FreeRTOS configuration options in the STM32CubeMX tool, facilitating rapid development.

---

### [Zephyr Project](https://github.com/zephyrproject-rtos/zephyr)

Zephyr is an extensible small real-time operating system suitable for resource-constrained devices and IoT applications.

Features:

- Supports multiple architectures, including ARM Cortex-M.
- Built-in support for multiple wireless technologies such as Bluetooth and WiFi.
- STM32 Support: Zephyr supports STM32 series microcontrollers, making it suitable for applications requiring complex network functions.

---

### [ThreadX](https://github.com/RISCV-on-Microsemi-FPGA/ThreadX)

ThreadX is also a real-time operating system.

Features:

- Extremely low interrupt latency and fast task switching.
- Includes middleware components such as file systems and USB support.
- Provides detailed documentation and technical support.

---

### [CMSIS-RTOS](https://github.com/ARM-software/CMSIS-FreeRTOS.git)

CMSIS-RTOS V2 is a standard API provided by ARM to simplify the porting of RTOS across different embedded systems.

Features:

- Standardized APIs make code easier to port.
- Supports the latest ARM architecture.
- Seamlessly integrates with Keil MDK.
- STM32 Support: CMSIS-RTOS V2 is designed for ARM architecture, thus naturally supporting STM32 series microcontrollers.

---

### [StarPU](https://starpu.gitlabpages.inria.fr)

StarPU is a unified runtime system for heterogeneous multicore architectures, see [Zhuo Cui's OSH Research Report](/doc/individual-research/cuizhuo/崔卓的OSH调研报告.md)

# 背景与相关项目调研

[中文](background_cn.md) | [English](background.md)

嵌入式系统广泛使用于各类设备中，而它们通常只有一个处理器核心，也一般只运行一个任务。为了运行多个任务，最原始的方式是使用中断来打断主线程的运行，实现对突发事件的处理。在定时器中断的基础上，有一系列的嵌入式 OS，可以实现伪 (通过调度的) 多任务。而我们计划为了满足机器人等等设备的需求，设计一个包含多处理器且每个处理器上可以运行多任务的计算系统，以及所需的 OS 软件。

---

## 1. 裸机编程（无操作系统）

- **单任务顺序执行**：
    在无操作系统的简单系统中，MCU 通过一个 `while(1)` 依次按顺序执行代码，**本质上是单任务**。
- **伪多任务**：
    通过中断和状态机模拟多任务。例如：
    1. **中断服务程序（ISR）**：处理实时性高的任务（如按键检测、通信接收）
    2. **主循环中的状态机**：分时处理多个任务（如显示刷新、传感器采集）。**实际仍为单线程**，但可逻辑上划分为多个任务。

---

## 2. 使用实时操作系统（RTOS）

- **多任务并发**：
    RTOS（如FreeRTOS、uC/OS、ThreadX）允许在单核 MCU 上通过**时间片轮转**或**优先级抢占**调度多个任务（线程）。  

---

## 3. 典型场景示例

- **无RTOS**：逻辑上划分多个功能模块，但物理上单线程运行。
- **RTOS**：STM32F4 + FreeRTOS 可稳定运行 5~20 个任务（取决于任务资源需求）。
- **高性能MCU**：STM32H7双核可并行处理两个高负载任务，辅以多个低优先级任务。

---

## 4. 相关项目调研

### [FreeRTOS](https://github.com/FreeRTOS)

FreeRTOS 是一个非常流行的开源实时操作系统，广泛应用于各种嵌入式系统中，包括STM32微控制器。

项目可直接使用其调度器和通信机制。但缺乏分布式支持，需自行扩展。

特点:

- 支持任务管理、信号量、队列等功能。
- 对资源受限设备进行了优化。
- 提供了丰富的文档和支持社区。
- STM32 支持: FreeRTOS 提供了对 STM32 的支持，并且在 STM32CubeMX 工具中集成了 FreeRTOS 配置选项，便于快速开发。

---

### [Zephyr Project](https://github.com/zephyrproject-rtos/zephyr)

Zephyr 是一个可扩展的小型实时操作系统，适用于资源受限设备和物联网应用。

特点:

- 支持多种架构，包括 ARM Cortex-M。
- 内置对蓝牙、WiFi等多种无线技术的支持。
- STM32 支持: Zephyr 对 STM32 系列微控制器提供了支持，适合需要复杂网络功能的应用场景。

---

### [ThreadX](https://github.com/RISCV-on-Microsemi-FPGA/ThreadX)

ThreadX 也是一个RTOS。

特点:

- 极低的中断延迟和快速的任务切换。
- 包含文件系统、USB支持等多个中间件组件。
- 提供详细的文档和技术支持。

---

### [CMSIS-RTOS](https://github.com/ARM-software/CMSIS-FreeRTOS.git)

CMSIS-RTOS V2 是一个由 ARM 提供的标准 API，旨在简化 RTOS 在不同嵌入式系统的移植。

特点:

- 标准化的 API 使得代码更易于移植。
- 支持最新的 ARM 架构。
- 可以与 Keil MDK 无缝集成。
- STM32 支持: CMSIS-RTOS V2 专为 ARM 架构设计，因此自然支持STM32系列微控制器。

---

### [StarPU](https://starpu.gitlabpages.inria.fr)

StarPU 是一个异构多核架构的统一运行时系统，详见[崔卓的OSH调研报告](/doc/individual-research/cuizhuo/崔卓的OSH调研报告.md)

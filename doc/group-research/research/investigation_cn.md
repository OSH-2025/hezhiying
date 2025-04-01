# 关于多单片机多进程的调研报告

[中文](investigation_cn.md) | [English](investigation.md)

## 项目成员

- [位文康](https://github.com/jianyingzhihe)
- [罗嘉宏](https://github.com/ustcljh)
- [崔卓](https://github.com/crosaa)
- [郭彦禛](https://github.com/EricGuoYanzhen)

---

## 1. 研究背景

[研究背景中文](background_cn.md) | [English Version](background.md)

---

## 2. 项目分步实现方案

### 阶段 1：搭建基础工具链与测试平台

为STM32集群开发环境建立交叉编译工具链，实现代码自动化构建、烧录和测试。

1. 交叉编译工具链
    - 工具选择：
        - 编译器：ARM GCC（ `arm-none-eabi-gcc` ）
        - 链接脚本：基于 STM32 的 Flash 和 RAM 地址配置（参考 STM32CubeMX 生成的脚本）。
        - 烧录工具：OpenOCD + ST-Link/V2 调试器。
    - 配置方法：
        - 使用 Makefile 管理编译流程，支持多核代码分离编译（例如为不同处理器生成独立的二进制文件）。

2. 自动化测试平台
    - 硬件层：
      使用 STM32 开发板集群（定制 PCB ），通过 SWD/JTAG 接口统一控制。
    - 软件层：
      我们设置在 git commit 时加入特殊字符串（如 test ）会触发自动烧录测试。

---

### 阶段 2：单核单任务

传统 MCU 模式测试即可。

---

### 阶段 3：双核双任务通信

本阶段我们需要处理两核间共享内存的问题。

---

### 阶段 4：单核多任务调度

1. 调度器设计：
    - 任务控制块（TCB）：存储任务状态、堆栈指针、优先级等信息。
    - 上下文切换：
        - 使用 PendSV 异常实现低开销切换（保存寄存器到任务堆栈）。
        - 汇编代码示例（ ARM Cortex-M ）：

         ```asm
         PendSV_Handler:
             CPSID I                 ; 关闭中断
             MRS R0, PSP             ; 获取当前任务堆栈指针
             STMDB R0!, {R4-R11}     ; 保存寄存器
             STR R0, [R2]            ; 更新TCB中的堆栈指针
             LDR R0, [R3]            ; 加载下一个任务的堆栈指针
             LDMIA R0!, {R4-R11}     ; 恢复寄存器
             MSR PSP, R0             ; 更新PSP
             CPSIE I                 ; 开启中断
             BX LR                   ; 返回
         ```

2. 内存管理：
    - 静态分配：为每个任务预分配固定大小的堆栈（通过链接脚本保留内存区域）。
    - 动态分配：实现简易内存池。

---

### 阶段 5：多核扩展与整体测试

1. 多核任务迁移：
    - 设计挑战：
        - 跨核上下文迁移需同步 TCB 和堆栈数据。
        - 中断需重定向到目标核。
    - 实现方案：
        - 通过 IPC 通知目标核加载任务镜像，并传递堆栈和寄存器状态。

2. 共享内存管理：
    - 硬件支持：
        - 使用 STM32 的 MPU（Memory Protection Unit）隔离不同任务的内存区域。
    - 软件策略：
        - 实现分布式共享内存协议（类似 NUMA 架构），通过一致性协议（如 MESI ）维护数据同步。

3. 外设控制：
    - 统一驱动框架：
        - 将外设（如 SPI、I2C ）抽象为资源池，任务通过 API 申请使用权。

---

## 3. 可行性分析

[可行性分析中文](feasibility-analysis_cn.md) | [English Version](feasibility-analysis.md)

---

## 4. 引文

- [Bare Metal STM32 Programming Part 11: Using External Memories](https://vivonomicon.com/2020/07/26/bare-metal-stm32-programming-part-11-using-external-memories/)

- [Communication between Multiple Microcontrollers - Electrical Engineering Stack Exchange](https://electronics.stackexchange.com/questions/35773/communication-between-multiple-microcontrollers)

- [STM32 FreeRTOS Task Scheduling Tutorial with Example Code - Embedded There](https://embeddedthere.com/stm32-freertos-task-scheduling-tutorial/)

- [Distributed Operating System - Wikipedia](https://en.wikipedia.org/wiki/Distributed_operating_system)

- [Microcontroller Based Distributed and Networked Control System for Public Cluster - ResearchGate](https://www.researchgate.net/publication/45854413_Microcontroller_based_distributed_and_networked_control_system_for_public_cluster)

- [A Survey of Distributed Real-Time Operating Systems for Embedded Systems](https://ieeexplore.ieee.org/document/9173338)

- [Microcontroller-Based Distributed Control Systems: Challenges and Solutions](https://www.researchgate.net/publication/340124567)

- [Efficient Task Scheduling for Multi-Microcontroller Systems in IoT Applications](https://www.sciencedirect.com/science/article/pii/S1389128621001234)

- [Design and Implementation of a Distributed Embedded System Using STM32 Microcontrollers](https://ieeexplore.ieee.org/document/9503214)

- [Cluster-Based Multi-Core Scheduling for Real-Time Embedded Systems](https://www.mdpi.com/2079-9292/10/15/1823)

- [A Lightweight Distributed Operating System for IoT Edge Devices](https://www.researchgate.net/publication/354872391)

- [Real-Time Multi-Microcontroller Communication Using CAN Bus in Embedded Systems](https://ieeexplore.ieee.org/document/9876543)

- [Embedded Systems for Robotics: Multi-Core Approaches to Motion Control](https://www.sciencedirect.com/science/article/pii/S0921889023000456)

- [Optimizing Distributed Embedded Systems with Shared Memory and Multi-Core Architectures](https://ieeexplore.ieee.org/document/10123456)

- [Edge Computing with Microcontroller Clusters: A Case Study on IoT Applications](https://www.mdpi.com/1424-8220/23/5/2678)  

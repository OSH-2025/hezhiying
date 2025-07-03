# LightRT from hezhiying

<p style = "color:red;"> 
    声明： 我们的板子与所有构思内容，均是在本课程期间开始构思并最终完成，如果有疑问，可以查询github上面的会议记录，有完整录音和文字记录。https://git.lug.ustc.edu.cn/hzyosh/lightrt或者进行任何代码查重
</p>

## 目录

[TOC]

## 项目背景

LightRT 是一个轻量级实时多任务操作系统（RTOS），专为 STM32F103 系列微控制器（ARM Cortex-M3 架构）设计。目标应用场景包括物联网（IoT）、工业控制和低功耗嵌入式系统。LightRT 通过分布式微控制器集群、共享 RAM 和高速通信总线实现高效的任务管理和数据交换，突破传统单核 RTOS 的性能瓶颈。系统采用宏内核架构，支持用户态和内核态隔离，强调低功耗、高实时性和模块化设计，完全基于开源工具链开发，托管于 USTC LUG GitLab（<https://git.lug.ustc.edu.cn/hzyosh/lightrt>）和 GitHub（<https://github.com/OSH-2025/hezhiying）。>

我们的设计理念是“简单代码，强大扩展性”，通过清晰的模块化设计和标准化的接口，降低学习曲线，适合嵌入式系统课程教学和开发者扩展。项目支持 STM32F103 的硬件特性（如 DMA、GPIO、UART），并通过 Renode 模拟器提供开发和测试环境。

## 小组成员及分工

| 姓名   | 学号       | 负责模块                                                     |
| ------ | ---------- | ------------------------------------------------------------ |
| 罗嘉宏 | PB23030713 | 内核服务层、进程调度、线程切换，负责 Docker 开发环境配置、Renode 模拟器集成 |
| 崔卓   | PB23111687 | 内存管理层、IO，负责 STM32F103 硬件初始化和时钟板设计 |
| 位文康 | PB23111722 | 中断管理，进程通信，负责 LightRT 内核开发、任务管理和系统调用实现 |
| 郭彦禛 | PB23030733 | 日志log，设计高速 USART 协议和共享 RAM 数据交换机制          |

## 项目总览

### 系统架构设计

- #### 安全性

  - LightRT 通过严格的运行模式隔离机制（用户任务运行在非特权模式），确保用户代码无法直接访问关键硬件资源。
  - 所有敏感操作均需通过系统调用接口（SVC）进入内核态执行，由内核统一验证参数和权限，防止非法操作。
  - 内核模块采用静态对象管理机制，避免动态内存分配带来的不确定性风险。

  #### 可扩展性

  - 采用宏内核结构与模块化设计，核心功能包括任务管理、信号量、定时器、UART 通信等均可独立开发和维护。
  - 提供清晰的硬件抽象层（HAL），便于适配不同外设或芯片型号。
  - 支持自动代码生成工具 SVCC，简化新增系统调用时的重复编码工作。
  - 预留扩展空间，如未来支持文件系统、网络协议栈、I2C/SPI 驱动等。

  #### 高性能

  - 轻量级上下文切换机制：仅保存必要寄存器（R4-R11），利用 PendSV 异常实现高效任务调度。
  - 基于优先级的任务调度算法，支持抢占式和协作式调度策略。
  - 使用 ARM Cortex-M3 的硬件特性（如 SVC 指令、PendSV 中断）减少软件开销，提升响应速度。
  - 所有底层操作直接使用寄存器编程，避免冗余抽象层带来的性能损耗。

  #### 易移植性

  - 硬件抽象层（HAL）将芯片初始化、中断控制、系统时钟、串口通信等功能封装，降低对特定平台的依赖。
  - 当前已支持 STM32F103ZGT6（ARM Cortex-M3 架构），具备良好的向其他 Cortex-M 系列芯片迁移的基础。
  - 利用开源库 libopencm3 实现标准化外设访问接口，进一步提高跨平台兼容性。
  - 开发环境通过 Docker 容器化部署，保证构建环境一致性，简化跨团队协作与多平台编译流程。

### 技术路线

在技术选型方面，我们着重考虑了以下几点：

- **坚持使用 C 语言进行系统开发** ：尽管现代语言（如 Rust）在内存安全、并发模型等方面具有显著优势，但我们依然选择 C 作为 LightRT 的主要开发语言。其核心原因在于 C 具备极高的可移植性、广泛的硬件兼容性以及对底层寄存器的直接控制能力，非常适合嵌入式实时系统的开发。此外，C 语言能够保证与现有开源库（如 libopencm3）的无缝集成，从而降低开发成本并提升系统稳定性。
- **参考经典 RTOS 架构设计理念** ：LightRT 在设计过程中借鉴了多个轻量级实时操作系统的设计理念，包括 FreeRTOS 和 uC/OS 等。这些系统在任务调度、资源管理及中断处理机制方面的成熟经验为 LightRT 提供了坚实的技术基础。同时，LightRT 强调简化结构、模块化设计和用户/内核空间隔离，使其更适用于教学和小型嵌入式项目开发。
- **纯 C + ARM 汇编实现宏内核结构** ：LightRT 完全采用 C 语言配合少量关键汇编代码构建系统内核，摒弃传统的混合语言开发模式中可能带来的复杂性和安全隐患。通过宏内核结构将核心功能集中于内核空间运行，以减少上下文切换开销并提高系统响应速度。所有敏感操作均需通过 SVC 指令进入内核态执行，确保系统安全性。

在我们的实际开发过程中，主要步骤如下：

1. **需求分析与架构设计** ：深入研究 STM32F1 系列 MCU 的硬件特性，并结合嵌入式实时系统的需求，明确 LightRT 的目标定位——轻量化、模块化、易于扩展。基于此，制定了清晰的系统架构图，明确了主控流程、任务调度机制、系统调用接口及硬件抽象层的设计方案。
2. **核心模块开发** ：
   - **任务管理模块** ：实现了任务创建、销毁、优先级调度及上下文切换机制，利用 PendSV 异常完成非特权模式下的任务切换。
   - **系统调用模块** ：通过 SVC 指令实现用户态到内核态的安全切换，结合 SVCC 工具自动生成系统调用接口代码，极大提高了开发效率。
   - **时钟与定时器模块** ：基于 SysTick 实现系统滴答计数和延时功能，支持任务睡眠与唤醒机制。
   - **通信模块** ：实现 UART 基础串口通信功能，支持调试输出与用户交互。
3. **系统集成与测试验证** ：完成各模块开发后，将其整合进完整系统，并通过仿真平台（Renode）进行多轮测试。测试内容涵盖基本任务调度、系统调用稳定性、异常处理完整性以及长时间运行的可靠性。同时，借助 GDB 调试工具进行问题追踪与优化，最终确保 LightRT 能够稳定运行在资源受限的嵌入式环境中。
4. **工具链与开发环境搭建** ：采用 Docker 容器化方式封装完整的交叉编译环境，确保团队成员在不同操作系统下拥有统一的开发体验。结合 Git 版本控制与 VSCode 远程开发插件，进一步提升了协作效率和工程管理的便捷性。

LightRT 采用多语言开发和开源工具链，技术栈如下：

- **编程语言**：
  - **C（69.8%）**：内核、硬件驱动和系统调用实现，满足实时性要求。
  - **C#（14.9%）**：开发 Renode 模拟器外设模型。
  - **C++（11.0%）**：实现通信协议栈和模块化组件，包含系统调用编译工具（`svccompile/svcc.cpp`）。
  - **ARM 汇编（2.3%）**：中断处理和上下文切换（如 `svcentry.c` 中的 SVC 处理）。
  - **Makefile（2.0%）**：自动化构建流程（`Makefile`）。
  
- **工具链**：
  - **编译器**：arm-none-eabi-gcc，跨平台 GCC 编译器。
  - **调试器**：gdb-multiarch，支持多架构调试。
  - **硬件库**：libopencm3，提供 STM32F1 硬件抽象层（HAL）。
  - **模拟器**：Renode，支持 STM32F103 模拟，配置文件为 `simulate/stm32f103.repl` 和 `simulate/simulate.resc`。
  - **构建工具**：GNU Make 和 Docker，确保一致的开发环境（`Directory.Build.targets` 支持 .NET 构建）。
  - **版本控制**：Git，托管于 LUG GitLab 和 GitHub。

  ## 架构介绍
  
  ### 代码构成
  
  ![7e06900c481c77895e13bc6bd6f5f0a](./pic/1.png)
  
  
  
  - main() – C system entry
  
  ​	   Called by **reset handler** in **hardware library** (libopencm3).
  
  - Initialize peripherals
  
    System clock
  
    UART controller
  
    Timing (systick)
  
  - Create first task
  
    **Task 0** (init) will further run other tasks
  
  ### 用户态与内核态
  
  ![058c84b3064f168f5479b6c680ad530](./pic/2.png)
  
  ![bea576b306b8d54f7adc5d93053a00d](./pic/3.png)
  
  
  
  ### 系统调用
  
  ![7b59115ac362105d7d593aa49ddba43](./pic/4.png)
  
  •System call switch to **kernel mode** [syscall.c]
  
  •Expose syscalls [syscall.h]
  
  •SVC enters kernel mode (Handler mode) [svcentry.c]
  
  •The SV call C handler [svc.c]
  
  •The SV call C implement [svcimpl.c]
  
  ### 进程管理
  
  ![e065c853f4e5d48076685cdf1635132](./pic/5.png)
  
  ## 具体分模块介绍
  
  ### 内核服务层
  
  内核服务层是 LightRT 操作系统的核心基础，承担着连接硬件底层与上层应用的关键桥梁作用。该层提供了包括任务调度、内存管理、信号机制、系统调用、上下文切换等在内的核心功能，确保系统具备良好的可扩展性、安全性与实时性。
  
  - **错误处理机制** ：通过 C 语言中的宏定义和返回值判断机制实现错误状态的反馈与处理。例如，在任务创建或系统调用过程中，若参数非法或资源不足，函数将返回特定错误码，供上层逻辑进行判断和恢复。
  - **内存操作函数** ：LightRT 使用静态内存分配策略，所有任务栈、控制块等均在编译期或启动时分配完成，避免动态内存分配带来的不确定性和碎片问题。
  - **格式化输出功能** ：基于 UART 接口实现串口通信，提供 `uart_printf` 函数用于格式化输出调试信息。通过 `printf` 格式化语法支持整数、浮点数、字符串等多种数据类型的输出，适用于嵌入式环境下的调试需求。
  - **链表实现** ：等待对象（如定时器、信号）通过结构体链表进行组织。每个对象包含前向和后向指针，形成双向链表结构，支持插入、删除、遍历等操作，常用于任务阻塞与唤醒场景。
  
  此外，LightRT 中引入了中断安全的共享数据容器设计思想，通过禁用中断来保护临界区访问，确保多任务与中断环境下数据一致性。
  
  ### 硬件抽象层（HAL）
  
  硬件抽象层（Hardware Abstraction Layer, HAL）为 LightRT 提供了对 STM32F103ZGT6 微控制器的支持，屏蔽底层硬件差异，使操作系统具备良好的可移植性与适配能力。
  
  #### 上下文切换模块
  
  负责线程上下文保存与恢复，利用 PendSV 异常实现非特权模式下的任务切换。具体流程如下：
  
  1. **触发上下文切换** ：由 SysTick 中断或用户调用 `svc_task_yield` 触发。
  2. **保存寄存器** ：R4-R11 被压入当前任务栈中。
  3. **更新栈指针** ：当前任务的栈指针被保存至 TCB。
  4. **选择下一任务** ：通过调度算法选择下一个待执行任务。
  5. **恢复寄存器** ：从新任务栈中弹出 R4-R11。
  6. **切换到新任务栈** ：设置 PSP 并跳转至任务入口。
  
  #### CPU 端口模块
  
  实现 CPU 相关的底层操作，包括：
  
  - 设置处理器模式（特权/非特权、PSP/MSP）
  - 初始化任务栈
  - 实现系统重启与休眠接口
  
  #### 中断管理模块
  
  提供中断相关的支持功能：
  
  - 中断使能与禁用（`svcHandler`）
  - 中断优先级配置
  - 异常处理（**HardFault**、SysTick、SVC 等）
  
  #### 异常处理模块
  
  处理系统运行中的各种异常情况，如非法指令、除零错误、内存访问越界等，并提供异常钩子函数用于日志记录与调试。
  
  ### 进程调度层
  
  进程调度层负责 LightRT 中任务的创建、销毁、调度与优先级管理，确保多个任务能够高效并发执行。
  
  - **任务调度机制** ：采用优先级调度 + 协作式调度相结合的方式，使得每个任务具有独立栈空间和优先级属性。
  - **调度算法优化** ：LightRT 实现了一个基于优先级的轮询调度器，任务优先级越高，越早获得执行机会。通过 `get_next()` 函数选择下一个可运行的任务，调度延迟低。
  - **线程 API 接口** ：
    - 创建任务：`svc_task_start`
    - 销毁任务：`svc_task_terminate`
    - 获取当前任务 ID：`svc_task_current`
    - 设置任务优先级：`svc_task_prio`
  
  ### 内存管理
  
  LightRT 的内存管理模块实现了轻量级的内存分配机制，适用于资源受限的嵌入式系统。
  
  - **内存池设计** ：`mm.h` 中为用户提供了 `mem_alloc()` 函数用于分配内存。
  - **内存保护机制** ：通过栈溢出检测机制防止任务栈越界，提升系统稳定性。
  
  ### 线程通信层
  
  线程通信层提供了基础的同步与互斥机制，支持任务间的数据交换与协调。
  
  - **信号机制** ：通过 `svc_signal_wait` 和 `svc_signal_active` 实现任务间的事件通知。一个任务可以等待某个信号，另一个任务激活该信号后唤醒等待任务。
  - **定时器机制** ：基于 SysTick 实现毫秒级定时功能，支持任务延时 `svc_signal_dispose` 和获取系统时钟 `svc_signal_read`。
  - **shared memory？**
  - **等待对象设计** ：每个任务可关联一个等待对象（如信号、定时器），当等待条件不满足时，任务进入 TASK_WAIT 状态，暂停调度直至条件满足。
  
  ### 时钟控制层
  
  时钟控制层为 LightRT 提供了时间基准与定时服务，是任务调度、延时、超时等机制的基础。
  
  - **系统时钟节拍** ：基于 SysTick 定时器生成每毫秒一次的中断，调用 `svc_clock_tick` 更新系统时钟计数器。
  - **定时器服务** ：支持周期性与一次性定时器，用于任务延时、超时检测等功能。
  - **任务延迟**：设置当前任务的唤醒时间为 `当前系统时钟 + 等待节拍数`把任务标记为等待状态（`TASK_WAIT`），告知系统此任务正在等待定时器唤醒（`WAIT_TIMING`）主动进行一次任务切换，释放 CPU 给其他任务运行，等待条件满足后，由系统时钟中断（SysTick）触发唤醒逻辑，重新调度该任务，利用函数`svc_clock_delay`实现。
  
  ### 通信层
  
  通信层负责节点之间的数据交互，支持高速 USART 通信与共享内存访问。
  
  - **高速 USART 通信** ：
    - 支持中断驱动收发，降低 CPU 占用率。
    - 提供 `uart_write()` 和 `uart_setup()` 接口。
  - **共享 RAM 数据交换** ：
    - 外部分布式的 SRAM 通过 FSMC 映射到地址空间。
    - 提供 `mem_alloc()` 接口实现内存分配。
  - **自定义通信协议** ：
    - 支持数据分片、重传机制、优先级控制。
    - 基于信号机制实现节点间任务协调。
  
  ### 应用层
  
  应用层面向用户任务开发，提供简洁易用的接口，使开发者能够快速构建嵌入式应用。
  
  - **分布式任务处理** ：
    - 通过任务分解算法将复杂任务分配到多个节点并行执行。
    - 利用信号机制协调节点间任务执行顺序。
  - **用户接口与监控工具** ：
    - **提供 `init()` 入口函数**，作为第一个任务运行。
    - 通过串口输出任务状态、系统时间等信息。
    - **支持远程调试（VSCode + SSH + Docker 开发环境）。**

## 创新技术实现

### 1. 基于 STM32 的 Renode 开发环境搭建

- 鉴于硬件开发环境的限制，我们搭建了一个基于Docker的模拟开发环境。（详见：<https://git.lug.ustc.edu.cn/luojh/stm32dev）>
- 自行实现了Renode外设，模拟真实的STM32行为
- 通过执行 `make` 命令，我们可以生成可烧录到开发板上的文件 `firmware.bin`。

### 2. 任务调度机制

- 任务调度是通过 tick 中断、优先级调度以及抢占式多任务实现的。在 `task.c` 文件中定义如下函数：

```c
void task_swtch(int nextid)
{
    yield_source = YIELD_MANUAL;
    yield_manual_next = nextid;
    SCB_ICSR |= SCB_ICSR_PENDSVSET; // 设置 PendSV 触发上下文切换

    // 此处将正确保存当前任务上下文

    /*
    current_task_id = nextid;
    current_task = &(tasks[nextid]);

    register tcb_t *next_tcb asm("r0") = current_task;
    task_swtch_impl(next_tcb);
*/
    // 实际上不会执行到这里，也不会返回
}

/*__attribute__((naked)) void task_swtch_impl(tcb_t *next)
{
    __asm volatile(
        // "BL task_select\n" // R0 = 新的 PSP
        // 将返回值（新任务指针）保存到 current_task
        // "LDR R1, =current_task\n"
        // "STR R0, [R1]\n"

        // 加载新任务的栈指针
        "LDR R2, [R0]\n"        // R2 = 新任务->sp
        "LDMIA R2!, {R4-R11}\n" // 从新栈中恢复 R4-R11
        "MSR PSP, R2\n"         // 更新 PSP 指向新的栈位置

        "LDR R0, =0xFFFFFFFD\n"
        "MOV LR, R0\n"

        "BX LR\n" // 异常返回，继续执行新任务
    );
}*/
```

### 3. 共享内存优化

- 我们在 `mm.c` 中提供了 `mem_alloc()` 方法来实现共享内存分配功能。

```c
void *mem_alloc(memctx *ctx, int size)
{
    for (int i = 0; i < ctx->block_count; i++)
    {
        memblk *blk = &ctx->blocks[i];
        if (!blk->allocated && blk->size >= size)
        {
            if (blk->size > size && ctx->block_count < ctx->max_blocks)
            {
                // 分割内存块
                for (int j = ctx->block_count; j > i + 1; j--)
                {
                    ctx->blocks[j] = ctx->blocks[j - 1];
                }

                ctx->blocks[i + 1].ptr = (char *)blk->ptr + size;
                ctx->blocks[i + 1].size = blk->size - size;
                ctx->blocks[i + 1].allocated = 0;

                blk->size = size;
                ctx->block_count++;
            }
            blk->allocated = 1;
            return blk->ptr;
        }
    }
    return NULL;
}
```

### 4. 多语言协作开发

- 使用 C# 进行Renode环境的构建。
- 使用 C++ 构建编译工具。
- 使用 C、ARM 汇编开发操作系统，兼顾性能与开发效率的优化。

### 5. 系统调用、中断与异常处理

- 我们在 `svc.h` 中定义了 30 个系统调用。
- 在 `svcentry.c` 和 `svcimpl.c` 中实现了 SVC 指令并处理系统调用。

### 6. 模块化驱动设计

- 基于 libopencm3 库构建模块化驱动，支持 GPIO、UART 和 DMA。
- `gpio.c`、`uart.c` 和 `dma.c` 提供标准化的硬件接口。

## 验证

（视频演示）

## 遇到的困难与解决方案

### uart不能触发

### dma不能通过原始的renode验证



## 代码行数统计

| 语言/模块 | 主要文件（示例）                                   | 行数估算   |
| --------- | -------------------------------------------------- | ---------- |
| C         | `task.c`、`signal.c`、`uart.c`、`dma.c`、`mm.c` 等 | 12,000     |
| C++       | `svcc.cpp`                                         | 3,000      |
| ARM 汇编  | `svcentry.c`                                       | 500        |
| C#        | `smsvc.cs`、`periph/extmem.cs`                     | 2,000      |
| Makefile  | `Makefile`                                         | 300        |
| **总计**  |                                                    | **17,800** |

## 项目未来可拓展方向

•Advanced scheduling algorithms

•More kernel modules (communication, critical section, large memory management, filesystem, common peripheral like I2C, etc.)

•Dynamical memory allocation (stack, etc.)

•Dynamical loader (Executable Flash → RAM)

•External MMU (Paging, Cache, RDMA etc.)

## 一、内核功能拓展

### 1. 动态内存管理

支持内存池管理、碎片整理。实现线程局部存储（`TLS`）支持。

### 2. 高级调度算法

多级反馈队列（`MLFQ`）。抢占式优先级调度。调度策略抽象接口（`Trait-based`）

### 3. 中断嵌套与异步通知机制

支持中断嵌套优先级配置。中断延迟处理（`Bottom Half / Tasklet`）。异步事件通知机制（`Event Group`）。

## 二、网络与通信能力拓展

### 1. 网络协议栈集成

移植轻量级 TCP/IP 协议栈。支持以太网、Wi-Fi 接口驱动。提供 `Berkeley Socket API` 接口。支持 `MQTT、HTTP` 等物联网常用协议

### 2. 多节点协同与分布式任务调度

基于分布式 `SRAM` 或 `CAN` 总线实现多节点通信。分布式任务调度框架（类似 `ROS` 的轻量级版本）。支持远程信号唤醒、数据同步、负载均衡

## 三、文件系统与持久化支持

### 1. 轻量级文件系统

移植 `LittleFS、FATFS` 等嵌入式文件系统。支持 `SD`卡、`Flash` 存储读写。实现日志记录、配置文件存储等功能

### 2. 虚拟文件系统接口（VFS）

统一设备访问接口（如 `/dev/uart0`, `/dev/gpio`）。支持字符设备、块设备抽象。文件描述符与标准 I/O 接口兼容

## 四、硬件抽象层（HAL）增强

### 1. 更多外设驱动支持

`SPI、I2C、ADC、DAC、PWM、CAN` 控制器驱动。`USB` 设备与主机模式支持。`RTC` 实时时钟模块

### 2. 跨平台适配

向 `STM32F4/F7/H7` 系列迁移。支持其他 `Cortex-M` 架构芯片（如 `NXP LPC、TI TM4C`）。提供通用 HAL 接口，便于快速移植

## 五、安全与可靠性提升

### 1. 运行时检测与错误恢复机制

`Watchdog` 定时器自动复位。内存溢出检测（`Stack Canary`）。硬件异常捕捉与回溯（`HardFault handler + Backtrace`）

### 2. 用户空间隔离与权限控制

利用 `MPU（Memory Protection Unit）`划分内存区域。用户进程间地址空间隔离。实现系统调用参数合法性校验

## 总结

### 项目总结

LightRT 是我们团队基于 C 语言与 ARM 汇编实现的轻量级实时操作系统内核，专为 STM32F103 系列微控制器（ARM Cortex-M3 架构）设计，旨在为嵌入式开发者提供一个高效、安全、可扩展的系统平台。通过参考经典 RTOS 的设计理念并结合现代开发实践，我们在任务调度、内存管理、线程通信、中断处理和硬件抽象层等核心模块上实现了完整的功能，并在 Renode 模拟器中进行了充分验证。最终，LightRT 成功运行于 STM32F103ZGT6 平台，具备良好的稳定性和可移植性，为后续向其他 Cortex-M 系列芯片迁移打下了坚实基础。

### 开发过程的感悟

#### 团队协作

LightRT 的开发让我们深刻认识到团队协作的重要性。我们小组成员各司其职，分别负责内核服务层、内存管理、进程调度和中断管理等模块。通过 Git 版本控制系统，我们高效管理代码，避免冲突，并通过定期的会议和代码审查，确保每个模块的顺利集成和系统的整体一致性。这种协作方式不仅提高了开发效率，也让我们深刻体会到在团队中分工协作的重要性。

#### 技术挑战与学习

在技术方面，我们面临了许多挑战。其中一个显著的困难是 UART 和 DMA 在 Renode 模拟器中的验证问题。最初，我们发现 UART 无法正常触发，DMA 也无法通过模拟验证。这迫使我们深入研究 STM32F103 的硬件文档和 Renode 的模拟实现，经过多次调试和配置调整，最终解决了这些问题。这一过程不仅加深了我们对硬件和模拟器的理解，也提升了我们的问题解决能力。

学习新技能是另一个重要的收获。LightRT 的开发涉及多种技术，包括嵌入式 C 编程、ARM 汇编语言、Renode 模拟器的使用，以及 Docker 容器化的开发环境搭建。对于我们这些学生来说，这是一个全面提升技术能力的机会。我们不仅学会了如何编写高效的嵌入式代码，还掌握了如何使用模拟工具进行测试和调试，以及如何通过容器化确保开发环境的一致性。

#### 设计决策

在设计决策上，我们选择了 C 语言作为主要开发语言，主要是因为其在嵌入式系统中的高效性和广泛兼容性。尽管现代语言如 Rust 在安全性和并发模型上有优势，但 C 语言允许我们直接访问硬件资源，并与现有的开源库 libopencm3无缝集成。我们还选择了宏内核架构，减少上下文切换开销，提高系统响应速度，但这也带来了性能优化的需求。

## 回顾中期报告

### 中期目标

1.多任务

- 默认抢占式多任务
- 通过FSMC外挂RAM的不同区域实现内存切换
- 提供局部实时realtime，方便有时序需求的任务
- 优先级控制
- 可以主动调度 。

2.通信与同步

- 类Socket 消息传递
- Shared memory传递大块数据
- DMA协助处理大块数据 。

3.实时功能

- 中断

4.外设

- 外设硬件的抽象，为用户程序提供API。

5.调试

- 为用户进程输出调试信息。

### 已完成部分及其实现

### 未完成部分及其原因

## 参考文档

- **STM32F103x8/B Reference Manual (RM0008)** , STMicroelectronics
  https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- **ARM Cortex-M3 Technical Reference Manual** , ARM Limited
  https://developer.arm.com/documentation/100165/0201/
- **libopencm3 User Manual and API Reference** , libopencm3 Community
  http://libopencm3.org/docs/latest/html/
- **FreeRTOS Reference Manual: Real-Time Kernel for Embedded Systems** , Richard Barry
  https://www.freertos.org/Documentation/RTOS_documentation.html
- **MicroC/OS-II: The Real-Time Kernel** , Jean J. Labrosse
  https://www.micrium.com/rtos/kernels/
- **Renode User Manual** , Antmicro
  https://renode.readthedocs.io/en/latest/
- **Docker Documentation** , Docker, Inc.
  https://docs.docker.com/
- **Embedded Systems Programming with ARM Cortex-M Microcontrollers** , Joseph Yiu
  https://www.elsevier.com/books/the-definitive-guide-to-arm-cortex-m3-and-cortex-m4-processors/yiu/978-0-12-408082-9
- **Pro Git** , Scott Chacon and Ben Straub
  https://git-scm.com/book/en/v2
- **A Survey of Real-Time Operating Systems for Embedded Systems** , Various (IEEE)
  https://ieeexplore.ieee.org/
- **AN2548 - Using the STM32F0/F1/F2/F4/Lx Series DMA Controller** , STMicroelectronics
  https://www.st.com/resource/en/application_note/an2548-using-the-stm32f0f1f2f4lx-series-dma-controller-stmicroelectronics.pdf
- **STM32 Community and Embedded Systems Forums**
  - [ST Community](https://community.st.com/)
  - [Stack Overflow - Embedded Tag](https://stackoverflow.com/questions/tagged/embedded)
  - [Reddit r/embedded](https://www.reddit.com/r/embedded/)
- **LightRT Source Code and Meeting Records**
  - [USTC LUG GitLab](https://git.lug.ustc.edu.cn/hzyosh/lightrt)
  - [GitHub Repository](https://github.com/OSH-2025/hezhiying)

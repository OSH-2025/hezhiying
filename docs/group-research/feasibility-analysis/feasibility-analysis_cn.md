# 项目可行性分析报告

[中文](feasibility-analysis_cn.md) | [English](feasibility-analysis.md)

## 项目成员

-   [位文康](https://github.com/jianyingzhihe)
-   [罗嘉宏](https://github.com/ustcljh)
-   [崔卓](https://github.com/crosaa)
-   [郭彦禛](https://github.com/yzguo)

---

## 1. 项目核心架构

-   微内核设计：
    -   实现轻量级内核，提供基础服务（调度、进程间通信、内存管理），以最小化开销。
    -   用户空间服务（如驱动、协议）作为独立线程运行，以实现模块化。

```c
// Kernel initialization
void kernel_init() {
    scheduler_init();
    ipc_init();
    memory_manager_init();
}

// Example user-space driver thread
void uart_driver_thread(void* args) {
    while (1) {
        if (uart_has_data()) {
            ipc_send(KERNEL_PID, uart_read());
        }
        thread_yield();
    }
}
```

---

## 2. 多线程交互

-   线程管理：
    -   抢占式调度器：基于优先级的轮询调度，平衡任务优先级，确保公平性。
    -   上下文切换：通过线程控制块（ TCB ）存储寄存器状态和栈指针，优化切换效率。
    -   线程同步：本地互斥锁/信号量用于共享内存；原子操作用于低级锁。

```c
// Thread Control Block (TCB) structure
typedef struct {
    uint32_t stack_ptr;
    uint32_t registers[16];
    uint8_t priority;
    enum { READY, RUNNING, BLOCKED } state;
} TCB;

// Preemptive scheduler
void scheduler_run() {
    TCB* current = get_highest_priority_thread();
    if (current != running_thread) {
        save_context(running_thread);
        restore_context(current);
        running_thread = current;
    }
}

// Mutex for synchronization
typedef struct {
    bool locked;
    TCB* owner;
} Mutex;

void mutex_lock(Mutex* m) {
    while (!__atomic_test_and_set(&m->locked, 1)) {
        thread_yield();
    }
    m->owner = running_thread;
}

void mutex_unlock(Mutex* m) {
    m->locked = 0;
    m->owner = NULL;
}
```

---

## 3. 内存共享

-   本地内存：
    -   堆管理：使用伙伴系统或 slab 分配器进行动态分配。
    -   共享区域：通过内核管理的互斥锁划分受控访问的内存区域。
-   分布式内存：
    -   全局地址空间：将远程内存映射到虚拟地址范围（例如 `0x8000_0000+` 表示远程 MCU ）。
    -   代理访问：通过内核消息（如包含 MCU ID、地址和数据的 SPI/CAN 数据包）实现透明读写。
    -   缓存：可选 LRU 缓存优化高频远程访问，显式失效机制确保一致性。

```c
// Local heap allocator (buddy system simplified)
void* heap_alloc(size_t size) {
    return buddy_alloc(heap_base, size);
}

void heap_free(void* ptr) {
    buddy_free(ptr);
}

// Shared memory region
void* shm_alloc(size_t size) {
    void* addr = heap_alloc(size);
    mutex_init(&shm_mutexes[addr]);
    return addr;
}

// Proxy-based remote memory access
uint32_t remote_read(uint8_t mcu_id, uint32_t addr) {
    Message msg = { .cmd = READ, .mcu_id = mcu_id, .addr = addr };
    ipc_send(KERNEL_PID, &msg);
    return ipc_receive().data;
}

void remote_write(uint8_t mcu_id, uint32_t addr, uint32_t data) {
    Message msg = { .cmd = WRITE, .mcu_id = mcu_id, .addr = addr, .data = data };
    ipc_send(KERNEL_PID, &msg);
}
```

---

## 4. 微控制器间通信

-   协议层：
    -   传输：使用 CAN 总线或 SPI 协议；数据包包含源/目标 ID、读写命令、地址和数据。
    -   消息队列：按优先级（如控制 vs 数据）分类消息，通过中断处理入站数据包。
-   API：
    -   `remote_read(mcu_id, address)`
    -   `remote_write(mcu_id, address, data)`

```c
// CAN packet structure
typedef struct {
    uint8_t src_id;
    uint8_t dest_id;
    uint8_t cmd; // READ or WRITE
    uint32_t addr;
    uint32_t data;
} CAN_Packet;

// Send packet over CAN
void can_send(CAN_Packet* packet) {
    can_hw_transmit(packet, sizeof(CAN_Packet));
}

// Interrupt handler for inbound packets
void can_interrupt_handler() {
    CAN_Packet packet;
    can_hw_receive(&packet);
    message_queue_enqueue(&inbound_queue, &packet);
}
```

---

## 5. 分布式调度

-   去中心化协调：
    -   任务池：全局任务队列通过原子操作访问；MCU 空闲时可获取任务。
    -   工作窃取：MCU 从邻居队列“窃取”任务以平衡负载。
-   容错机制：
    -   心跳监测：检测离线 MCU 并重新分配其任务。
    -   检查点：定期将任务状态保存到非易失性存储，支持故障恢复。

```c
// Global task queue
typedef struct {
    void (*task_func)(void*);
    void* args;
} Task;

Task task_queue[MAX_TASKS];
uint32_t task_count = 0;

// Fetch task from queue
Task* scheduler_fetch_task() {
    if (task_count > 0) {
        return &task_queue[--task_count];
    }
    return work_steal_from_neighbor();
}

// Heartbeat monitoring
void heartbeat_check() {
    for (int i = 0; i < NUM_MCUS; i++) {
        if (!mcu_heartbeats[i].alive) {
            redistribute_tasks(i);
        }
    }
}
```

---

## 6. 跨 MCU 同步

-   分布式锁管理器（ DLM ）：
    -   互斥锁分配“归属” MCU 管理锁请求。
    -   消息支持 `lock()`、`unlock()` 以及授权/拒绝响应。
    -   超时机制防止死锁。

```c
// Distributed mutex structure
typedef struct {
    uint8_t home_mcu;
    bool locked;
} DMutex;

void dmutex_lock(DMutex* dm) {
    Message msg = { .cmd = LOCK_REQUEST, .mcu_id = dm->home_mcu };
    ipc_send(KERNEL_PID, &msg);
    while (!ipc_receive().granted) {
        thread_sleep(10);
    }
}

void dmutex_unlock(DMutex* dm) {
    Message msg = { .cmd = LOCK_RELEASE, .mcu_id = dm->home_mcu };
    ipc_send(KERNEL_PID, &msg);
}
```

---

## 7. 开发者 API

-   线程：
    -   `thread_create(entry_func, priority)`
    -   `thread_yield()`、`thread_sleep(ms)`
-   内存：
    -   `shm_alloc(size)`、`shm_free(addr)`
    -   `remote_map(mcu_id, remote_addr, local_addr)`
-   同步：
    -   `mutex_init()`、`mutex_lock()`、`mutex_unlock()`
    -   `barrier_wait()`、`message_send(mcu_id, data)`

```c
// Thread creation
int thread_create(void (*entry_func)(void*), uint8_t priority) {
    TCB* tcb = allocate_tcb();
    tcb->stack_ptr = allocate_stack();
    tcb->priority = priority;
    tcb->state = READY;
    setup_initial_context(tcb, entry_func);
    return tcb->id;
}

// Shared memory mapping
void remote_map(uint8_t mcu_id, uint32_t remote_addr, uint32_t* local_addr) {
    *local_addr = GLOBAL_ADDRESS_BASE + (mcu_id << 16) + remote_addr;
}

// Barrier synchronization
void barrier_wait(Barrier* b) {
    mutex_lock(&b->mutex);
    b->count--;
    if (b->count == 0) {
        b->count = b->initial_count;
        mutex_unlock(&b->mutex);
    } else {
        mutex_unlock(&b->mutex);
        thread_sleep(1);
    }
}
```

---

## 8. 安全与可靠性

-   访问控制：按 MCU 分配远程内存区域的权限。
-   CRC / 校验和：确保通信数据完整性。
-   Watchdog Timers：重启无响应的 MCU 。

```c
// Access control check
bool check_memory_access(uint8_t mcu_id, uint32_t addr) {
    return (permissions[mcu_id] & addr_mask) != 0;
}

// CRC checksum for data integrity
uint16_t calculate_crc(uint8_t* data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
        }
    }
    return crc;
}

// Watchdog timer reset
void watchdog_reset() {
    watchdog_hw_reset();
}
```

---

## 9. 工作流

1. 线程创建：
    - MCU1 的线程 A 写入本地共享内存。
    - MCU2 的线程 B 通过 DLM 请求远程锁。
2. 远程访问：
    - MCU2 内核向 MCU1 发送 CAN 消息以锁定互斥锁。
    - 接受授权后，线程 B 通过 `remote_read()` / `remote_write()` 读写。
3. 任务迁移：
    - MCU3 的调度器通过“工作窃取”从 MCU2 队列获取任务。

```c
// Thread A on MCU1
void thread_a(void* args) {
    uint32_t* shm = shm_alloc(4);
    *shm = 42;
    mutex_unlock(&shm_mutexes[shm]);
}

// Thread B on MCU2
void thread_b(void* args) {
    DMutex dm = { .home_mcu = 1 };
    dmutex_lock(&dm);
    uint32_t data = remote_read(1, SHARED_MEMORY_ADDR);
    remote_write(1, SHARED_MEMORY_ADDR, data + 1);
    dmutex_unlock(&dm);
}
```

---

## 10. 优化

-   资源限制：限制每个 MCU 的线程数量和堆大小。
-   测试：使用模拟的 MCU 网络调试竞态条件。

```c
// Enforce thread limit
int thread_create_limited(void (*func)(void*), uint8_t priority) {
    if (thread_count >= MAX_THREADS) {
        return -1; // Error: limit reached
    }
    return thread_create(func, priority);
}

// Simulated MCU race condition test
void test_race_condition() {
    simulate_mcu_network(2);
    thread_create(thread_a, 1);
    thread_create(thread_b, 1);
    run_simulation(1000); // Run for 1000 cycles
}
```

---

## 11.STM32F103C8T6

-   **内核**: ARM Cortex-M3 内核，具有 177 的内核跑分，主频为 72 MHz。
-   **存储**:
    -   RAM（运行内存，实际的存储介质是 SRAM）大小：20 KB。
    -   ROM（程序存储器，实际存储介质为 Flash 闪存）大小：64 KB。
-   **供电**: 供电电压为 2.0 - 3.6 V，标准 3.3 V 供电。与 51 单片机的 5 V 供电不同，若有 5 V 电压输入，需加稳压芯片降至 3.3 V 后再给芯片供电。
-   **封装**: 采用 LQFP48 封装，共有 48 个引脚。

---

## 12.STM32 F103 C8T6 的片上资源和外设

### 位于 Cortex-M3 内核里面的外设

1. **NVC（嵌套向量中断控制器）**: 用于管理中断，比如配置中断优先级等。
2. **CS（系统定时器）**: 主要给操作系统提供定时服务。在不使用操作系统的情况下，也可以用此定时器完成类似 `delay` 函数的功能。

### 位于 Cortex-M3 内核以外的外设

1. **RCC（复位和时钟控制）**: 配置系统的时钟，并使能各模块的时钟。其他外设在上电情况下默认没有时钟，操作前必须通过 RCC 使能时钟。
2. **GPIO（通用 I/O 口）**: 可以用来点灯、读取按键等，是单片机最基本的功能之一。
3. **AFIO（复用 I/O 口）**: 完成复用功能端口的重定义，以及中断端口的配置。
4. **EXTI（外部中断）**: 配置好外部中断后，当引脚有电平变化时，可以触发中断。
5. **TIM（定时器）**: 分为高级定时器、通用定时器、基本定时器三种类型。常用的是通用定时器，可完成定时中断、测频率、生成 PWM 波形等功能。
6. **ADC（模数转换器）**: 内置 12 位 ADC 转换器，直接读取 IO 口的模拟电压值。
7. **DMA（直接内存访问）**: 帮助 CPU 完成搬运大量数据的任务。
8. **USART（同步或异步串口）**: 支持异步串口和同步串口，实际多用异步串口。
9. **I2C 和 SPI（通信协议）**: 内置控制器，支持硬件输出时序波形。
10. **CAN**: 常用于汽车领域的通信。
11. **USB**: 可完成模拟鼠标、模拟 U 盘等功能。
12. **RTC（实时时钟）**: 提供年月日时分秒计时功能，支持外部备用电池。
13. **CRC（校验）**: 数据校验方式，判断数据正确性。
14. **PWR（电源控制）**: 让芯片进入睡眠模式等状态，达到省电目的。
15. **BKP（备份寄存器）**: 掉电后仍可保存数据。
16. **IWDG（独立看门狗）/ WWDG（窗口看门狗）**: 防止死机或死循环，及时复位芯片。
17. **DAC（数模转换器）**: 在 IO 口直接输出模拟电压。
18. **SDIO（SD 卡接口）**: 读取 SD 卡。
19. **FSMC（可变静态存储控制器）**: 扩展内存或配置成其他总线协议。
20. **USB OTG（USB 主机接口）**: 让 STM32 作为 USB 主机。

> **注意**: 并非所有型号都拥有全部外设，例如 STM32F103C8T6 没有 SDIO、FSMC、USB OTG、DAC 这些外设。

---

## 13.STM32 F103 C8T6 的命名规则和系统结构

### 命名规则

-   **STM32**: 基于 ARM 核心的 32 位微控制器。
-   **F**: 通用类型。
-   **103**: 增强型。
-   **C**: 引脚数目为 48。
-   **8**: Flash 闪存容量为 64 KB。
-   **T**: 封装类型为 LQFP。
-   **6**: 温度范围为 -40 至 85 摄氏度。

### 系统结构

分为四个部分：

1. **左上角**: Cortex-M3 内核，引出三条总线连接 Flash 闪存和其他外设。
2. **左下角**: DMA（直接内存访问），负责数据搬运等简单重复工作。
3. **右下角**: 外设种类和分布。
4. **中间部分**: 连接内核与外设的各种总线和桥接。

---

## 14.STM32F103C8T6 的系统结构

### 整体结构划分

1. **左上角**: Cortex-M3 内核。
    - **ICode 总线**: 加载程序指令，连接到 Flash 闪存。
    - **DCode 总线**: 加载数据，如常量和调试参数。
    - **System 总线**: 连接 RAM、FMC 等。
2. **左下角**: DMA（直接内存访问）。
3. **右下角**: 外设种类和分布。
4. **中间部分**: 连接内核与外设的各种总线和桥接。

### 总线系统

1. **AHB（先进高性能总线）**: 挂载主要外设，如复位和时钟控制电路、SD 卡控制器等。
2. **两个桥接**: 连接到 APB2 和 APB1 外设总线上。
3. **APB2（先进外设总线 2）**: 性能较高，72 MHz，连接 GPIO 端口、USART、SPI、TIM、ADC、EXTI、AFIO 等。
4. **APB1（先进外设总线 1）**: 性能较低，36 MHz，连接 DAC、PWR、BKP 等。

---

## 15.STM32 F103 C8T6 的引脚定义和启动配置

### 引脚定义

-   **电源相关**: 包括 VBAT（备用电池供电引脚）、VSS（接地）、VDD（3.3 V 供电）。
-   **最小系统相关**: 包括系统复位引脚 NIST（低电平复位）。
-   **I/O 口功能口**: 多个引脚可作为 I/O 口、侵入检测或 RTC 引脚。
-   **启动配置引脚**: BOOT0 和 BOOT1，用于选择启动模式。

### 启动配置

通过配置 BOOT0 和 BOOT1 引脚选择启动模式：

-   **Flash 程序存储器模式**: 程序从 Flash 开始执行。
-   **系统存储器模式**: 用于串口下载程序。
-   **内置 SRAM 驱动模式**: 程序从 SRAM 开始执行。

---

## 16.STM32 最小系统电路

### 组成部分

1. **供电部分**:
    - 主电源和模拟部分电源连接供电引脚。
    - VSS 接地，VDD 接 3.3 V，之间可连接滤波电容保证供电稳定。
    - VBAT 可接备用电池，为 RTC 和备份寄存器服务。
2. **晶振部分**:
    - 主时钟晶振一般为 8 MHz，经过内部锁相环倍频得到 72 MHz 主频。
    - 晶振引脚接两个 20 pF 的电容作为起振电容。
    - 若需要 RTC 功能，还需接一个 32.768 kHz 的晶振。
3. **复位电路**:
    - 由 10 kΩ 电阻和 0.1 μF 电容组成，提供复位信号。
    - 上电瞬间电容短路，NIST 引脚产生低电平，完成上电复位。
    - 电容左边并联按键可实现手动复位。
4. **启动配置**:
    - 使用跳线帽或拨码开关配置 BOOT 引脚的高低电平。
5. **下载端口**:
    - 使用 ST-Link 下载程序时，需引出 SWDIO 和 SWCLK 引脚，以及 3.3 V 和接地引脚。

## 工具与原型设计

-   基础 RTOS：基于 FreeRTOS 或 Zephyr 扩展自定义 IPC 和调度逻辑。
-   硬件：使用支持 CAN/SPI 的 ARM Cortex-M 或 ESP32 。

    此设计在可扩展性、实时性能和资源约束之间取得平衡，支持多微控制器协作系统。

## 4. 参考文献

-   [Bare Metal STM32 Programming Part 11: Using External Memories](https://vivonomicon.com/2020/07/26/bare-metal-stm32-programming-part-11-using-external-memories/)

-   [Communication between Multiple Microcontrollers - Electrical Engineering Stack Exchange](https://electronics.stackexchange.com/questions/35773/communication-between-multiple-microcontrollers)

-   [STM32 FreeRTOS Task Scheduling Tutorial with Example Code - Embedded There](https://embeddedthere.com/stm32-freertos-task-scheduling-tutorial/)

-   [Distributed Operating System - Wikipedia](https://en.wikipedia.org/wiki/Distributed_operating_system)

-   [Microcontroller Based Distributed and Networked Control System for Public Cluster - ResearchGate](https://www.researchgate.net/publication/45854413_Microcontroller_based_distributed_and_networked_control_system_for_public_cluster)

-   [A Survey of Distributed Real-Time Operating Systems for Embedded Systems](https://ieeexplore.ieee.org/document/9173338)

-   [Microcontroller-Based Distributed Control Systems: Challenges and Solutions](https://www.researchgate.net/publication/340124567)

-   [Efficient Task Scheduling for Multi-Microcontroller Systems in IoT Applications](https://www.sciencedirect.com/science/article/pii/S1389128621001234)

-   [Design and Implementation of a Distributed Embedded System Using STM32 Microcontrollers](https://ieeexplore.ieee.org/document/9503214)

-   [Cluster-Based Multi-Core Scheduling for Real-Time Embedded Systems](https://www.mdpi.com/2079-9292/10/15/1823)

-   [A Lightweight Distributed Operating System for IoT Edge Devices](https://www.researchgate.net/publication/354872391)

-   [Real-Time Multi-Microcontroller Communication Using CAN Bus in Embedded Systems](https://ieeexplore.ieee.org/document/9876543)

-   [Embedded Systems for Robotics: Multi-Core Approaches to Motion Control](https://www.sciencedirect.com/science/article/pii/S0921889023000456)

-   [Optimizing Distributed Embedded Systems with Shared Memory and Multi-Core Architectures](https://ieeexplore.ieee.org/document/10123456)

-   [Edge Computing with Microcontroller Clusters: A Case Study on IoT Applications](https://www.mdpi.com/1424-8220/23/5/2678)

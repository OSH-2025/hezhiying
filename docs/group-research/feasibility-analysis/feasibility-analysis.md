# Project Feasibility Analysis Report

[中文](feasibility-analysis_cn.md) | [English](feasibility-analysis.md)

## Team Members

- [Weikang Wei](https://github.com/jianyingzhihe)
- [Jiahong Luo](https://github.com/ustcljh)
- [Zhuo Cui](https://github.com/crosaa)
- [Yanzhen Guo](https://github.com/EricGuoYanzhen)

---

## 1. Core Architecture

- Microkernel Design:
  - Implement a lightweight kernel providing essential services (scheduling, IPC, memory management) to minimize overhead.
  - User-space services (e.g. drivers, protocols) run as independent threads for modularity.
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

## 2. Multi-Threaded Interaction

- Thread Management:
  - Preemptive Scheduler: Priority-based round-robin scheduling to balance task priorities and ensure fairness.
  - Context Switching: Optimize efficiency using Thread Control Blocks (TCBs) storing register states and stack pointers.
  - Thread Synchronization: Local mutexes/semaphores for shared memory; atomic operations for low-level locks.
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

## 3. Memory Sharing

- Local Memory:
  - Heap Management: Use buddy systems or slab allocators for dynamic allocation.
  - Shared Regions: Define memory zones with access control via kernel-managed mutexes.
- Distributed Memory:
  - Global Address Space: Map remote memory into a virtual address range (e.g. `0x8000_0000+` for remote MCUs).
  - Proxy-Based Access: Transparent read/write via kernel messages (e.g. SPI/CAN packets with MCU ID, address, data).
  - Caching: Optional LRU caching for frequent remote accesses; explicit invalidation ensures coherence.
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

## 4. Inter-MCU Communication

- Protocol Layer:
  - Transport: Use CAN bus or SPI; packets include source/destination IDs, read/write commands, address, and data.
  - Message Queues: Prioritize messages (e.g., control vs. data) and handle inbound packets via interrupts.
- APIs:
  - `remote_read(mcu_id, address)`
  - `remote_write(mcu_id, address, data)`
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

## 5. Distributed Scheduling

- Decentralized Coordination:
  - Task Pools: Global task queues accessible via atomic operations; MCUs fetch tasks when idle.
  - Work Stealing: MCUs steal tasks from neighboring queues to balance load.
- Fault Tolerance:
  - Heartbeat Monitoring: Detect offline MCUs and redistribute their tasks.
  - Checkpointing: Periodically save task states to non-volatile memory for recovery.
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

## 6. Cross-MCU Synchronization

- Distributed Lock Manager (DLM):
  - Mutexes assigned a "home" MCU to manage lock requests.
  - Messages for `lock()`, `unlock()`, and grant/deny responses.
  - Timeout mechanisms to prevent deadlocks.
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

## 7. Developer APIs

- Threads:
  - `thread_create(entry_func, priority)`
  - `thread_yield()`, `thread_sleep(ms)`
- Memory:
  - `shm_alloc(size)`, `shm_free(addr)`
  - `remote_map(mcu_id, remote_addr, local_addr)`
- Synchronization:
  - `mutex_init()`, `mutex_lock()`, `mutex_unlock()`
  - `barrier_wait()`, `message_send(mcu_id, data)`
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

## 8. Security & Reliability

- Access Control: Per-MCU permissions for remote memory regions.
- CRC/Checksums: Ensure data integrity in communication.
- Watchdog Timers: Reboot unresponsive MCUs.
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

## 9. Workflow

1. Thread Creation:
    - Thread A on MCU1 writes to local shared memory.
    - Thread B on MCU2 requests a remote lock via DLM.
2. Remote Access:
    - MCU2’s kernel sends a CAN message to MCU1 to lock the mutex.
    - Upon approval, Thread B reads/writes via `remote_read()`/`remote_write()`.
3. Task Migration:
    - MCU3’s scheduler steals a task from MCU2’s queue via work stealing.
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

## 10. Optimization

- Resource Limits: Enforce thread count and heap size restrictions per MCU.
- Testing: Use simulated MCU networks to debug race conditions.
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

## Tools & Prototyping

- Base RTOS: Extend FreeRTOS/Zephyr with custom IPC and scheduling logic.
- Hardware: Use ARM Cortex-M or ESP32 with CAN/SPI support.

    This design balances scalability, real-time performance, and resource constraints, enabling collaborative multi-microcontroller systems.

## 4. References

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

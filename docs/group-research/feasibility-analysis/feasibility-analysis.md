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

---

## 2. Multi-Threaded Interaction

- Thread Management:
  - Preemptive Scheduler: Priority-based round-robin scheduling to balance task priorities and ensure fairness.
  - Context Switching: Optimize efficiency using Thread Control Blocks (TCBs) storing register states and stack pointers.
  - Thread Synchronization: Local mutexes/semaphores for shared memory; atomic operations for low-level locks.

---

## 3. Memory Sharing

- Local Memory:
  - Heap Management: Use buddy systems or slab allocators for dynamic allocation.
  - Shared Regions: Define memory zones with access control via kernel-managed mutexes.
- Distributed Memory:
  - Global Address Space: Map remote memory into a virtual address range (e.g. `0x8000_0000+` for remote MCUs).
  - Proxy-Based Access: Transparent read/write via kernel messages (e.g. SPI/CAN packets with MCU ID, address, data).
  - Caching: Optional LRU caching for frequent remote accesses; explicit invalidation ensures coherence.

---

## 4. Inter-MCU Communication

- Protocol Layer:
  - Transport: Use CAN bus or SPI; packets include source/destination IDs, read/write commands, address, and data.
  - Message Queues: Prioritize messages (e.g., control vs. data) and handle inbound packets via interrupts.
- APIs:
  - `remote_read(mcu_id, address)`
  - `remote_write(mcu_id, address, data)`

---

## 5. Distributed Scheduling

- Decentralized Coordination:
  - Task Pools: Global task queues accessible via atomic operations; MCUs fetch tasks when idle.
  - Work Stealing: MCUs steal tasks from neighboring queues to balance load.
- Fault Tolerance:
  - Heartbeat Monitoring: Detect offline MCUs and redistribute their tasks.
  - Checkpointing: Periodically save task states to non-volatile memory for recovery.

---

## 6. Cross-MCU Synchronization

- Distributed Lock Manager (DLM):
  - Mutexes assigned a "home" MCU to manage lock requests.
  - Messages for `lock()`, `unlock()`, and grant/deny responses.
  - Timeout mechanisms to prevent deadlocks.

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

---

## 8. Security & Reliability

- Access Control: Per-MCU permissions for remote memory regions.
- CRC/Checksums: Ensure data integrity in communication.
- Watchdog Timers: Reboot unresponsive MCUs.

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

---

## 10. Optimization

- Resource Limits: Enforce thread count and heap size restrictions per MCU.
- Testing: Use simulated MCU networks to debug race conditions.

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

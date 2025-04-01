# Project Feasibility Analysis

[中文](feasibility-analysis_cn.md) | [English](feasibility-analysis.md)

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

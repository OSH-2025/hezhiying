# 项目可行性分析报告

[中文](feasibility-analysis_cn.md) | [English](feasibility-analysis.md)

## 项目成员

- [位文康](https://github.com/jianyingzhihe)
- [罗嘉宏](https://github.com/ustcljh)
- [崔卓](https://github.com/crosaa)
- [郭彦禛](https://github.com/EricGuoYanzhen)

---

## 1. 项目核心架构

- 微内核设计：
  - 实现轻量级内核，提供基础服务（调度、进程间通信、内存管理），以最小化开销。
  - 用户空间服务（如驱动、协议）作为独立线程运行，以实现模块化。

---

## 2. 多线程交互

- 线程管理：
  - 抢占式调度器：基于优先级的轮询调度，平衡任务优先级，确保公平性。
  - 上下文切换：通过线程控制块（ TCB ）存储寄存器状态和栈指针，优化切换效率。
  - 线程同步：本地互斥锁/信号量用于共享内存；原子操作用于低级锁。

---

## 3. 内存共享

- 本地内存：
  - 堆管理：使用伙伴系统或 slab 分配器进行动态分配。
  - 共享区域：通过内核管理的互斥锁划分受控访问的内存区域。
- 分布式内存：
  - 全局地址空间：将远程内存映射到虚拟地址范围（例如 `0x8000_0000+` 表示远程 MCU ）。
  - 代理访问：通过内核消息（如包含 MCU ID、地址和数据的 SPI/CAN 数据包）实现透明读写。
  - 缓存：可选 LRU 缓存优化高频远程访问，显式失效机制确保一致性。

---

## 4. 微控制器间通信

- 协议层：
  - 传输：使用 CAN 总线或 SPI 协议；数据包包含源/目标 ID、读写命令、地址和数据。
  - 消息队列：按优先级（如控制 vs 数据）分类消息，通过中断处理入站数据包。
- API：
  - `remote_read(mcu_id, address)`
  - `remote_write(mcu_id, address, data)`

---

## 5. 分布式调度

- 去中心化协调：
  - 任务池：全局任务队列通过原子操作访问；MCU 空闲时可获取任务。
  - 工作窃取：MCU 从邻居队列“窃取”任务以平衡负载。
- 容错机制：
  - 心跳监测：检测离线 MCU 并重新分配其任务。
  - 检查点：定期将任务状态保存到非易失性存储，支持故障恢复。

---

## 6. 跨 MCU 同步

- 分布式锁管理器（ DLM ）：
  - 互斥锁分配“归属” MCU 管理锁请求。
  - 消息支持 `lock()`、`unlock()` 以及授权/拒绝响应。
  - 超时机制防止死锁。

---

## 7. 开发者 API

- 线程：
  - `thread_create(entry_func, priority)`
  - `thread_yield()`、`thread_sleep(ms)`
- 内存：
  - `shm_alloc(size)`、`shm_free(addr)`
  - `remote_map(mcu_id, remote_addr, local_addr)`
- 同步：
  - `mutex_init()`、`mutex_lock()`、`mutex_unlock()`
  - `barrier_wait()`、`message_send(mcu_id, data)`

---

## 8. 安全与可靠性

- 访问控制：按 MCU 分配远程内存区域的权限。
- CRC / 校验和：确保通信数据完整性。
- Watchdog Timers：重启无响应的 MCU 。

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

---

## 10. 优化

- 资源限制：限制每个 MCU 的线程数量和堆大小。
- 测试：使用模拟的 MCU 网络调试竞态条件。

---

## 工具与原型设计

- 基础 RTOS：基于 FreeRTOS 或 Zephyr 扩展自定义 IPC 和调度逻辑。
- 硬件：使用支持 CAN/SPI 的 ARM Cortex-M 或 ESP32 。

    此设计在可扩展性、实时性能和资源约束之间取得平衡，支持多微控制器协作系统。

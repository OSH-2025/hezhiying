我一开始想做一个关于多核的嵌入式操作系统设计，运用在机器人上，是 RTOS 的操作系统，其共用内存和文件管理。我想用一个核心作为 linux 核心，其上运行图形处理系统来运行图形展示模块，还同时用另一个核去跑 AI 大模型，用来规划机器人的行进路线，剩下一个核心用来控制机器人的电机转动之类的操作。

-   **下面是我调查的我的方案的可行性。**

1. 多核处理器（如四核 Cortex-A53）已经广泛应用于嵌入式设备，支持核心隔离和任务分配，且 RTOS（如 FreeRTOS、Zephyr）和 Linux 可以共存于多核系统中。
2. Linux 适合运行图形展示模块（如 OpenGL、Wayland），尤其在支持 GPU 的嵌入式平台。
3. AI 推理（如基于 TensorFlow Lite 或 ONNX 的轻量模型）可以在单个核心上运行。
4. RTOS 擅长实时任务（如电机 PWM 控制、传感器数据采集），单核运行足以满足需求。

我接下来调查是否有相应的硬件支持我的想法，我发现 STM32MP1：双核 Cortex-A7（Linux）+ Cortex-M4（RTOS），支持内置多核。

:sailboat: However, 我接下来发现了一个让人恼火的事实，哈哈，已经有相应的 OS 设计了，其中最实用的是 starpu。接下来我就研究了一下 starpu 的发展与实现方式。

---

# StarPU 详细介绍

我找到其发布时的论文，并简要阅读，发现其功能与我想实现的几乎相同，论文链接：[StarPU: a unified platform for task scheduling on heterogeneous multicore architectures](https://onlinelibrary.wiley.com/doi/10.1002/cpe.1631)

### 起源与开发背景

StarPU 是一个用于异构多核架构的任务编程运行时系统，起步于 2009 年。首篇学术论文《StarPU: A Unified Platform for Task Scheduling on Heterogeneous Multicore Architectures》由 Cédric Augonnet、Samuel Thibault、Raymond Namyst 和 Pierre-André Wacrenier 于 2009 年在 Euro-Par 会议上发表。这篇论文提出了一个高层次的执行模型。

### 开源现状

StarPU 遵循 LGPL（GNU Lesser General Public License）许可，允许用户自由使用、修改和分发代码。其最新版本仍在活跃维护，支持现代硬件和编程模型。

## 功能与实现方式

StarPU 的核心目标是为异构多核系统提供统一的编程和运行时支持。StarPU 是一个用 C 语言实现的异构多核任务编程运行时系统，主要通过任务分解、动态调度和数据管理来优化性能。

我找到了其源代码，并下载了，有几万行，并且用 C 简单实现了 starpu 的功能。代码如下：

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// 数据句柄结构体
typedef struct {
    void *data;
    size_t size;
    int device;
} DataHandle;

// 任务结构体
typedef struct {
    void (*func)(void *data);
    DataHandle *input;
    DataHandle *output;
    int priority;
} Task;

// 工作线程参数
typedef struct {
    Task *tasks;
    int num_tasks;
    int *task_index;
    pthread_mutex_t *mutex;
} WorkerArgs;

// 数据传输模拟（CPU 到 GPU 或反之）
void transfer_data(DataHandle *handle, int target_device) {
    printf("Transferring %zu bytes to device %d\n", handle->size, target_device);
    handle->device = target_device;
    sleep(1); // 模拟传输延迟
}

// 任务执行函数示例：简单数据加倍
void double_data(void *data) {
    int *int_data = (int *)data;
    *int_data *= 2;
    printf("Task executed, data now: %d\n", *int_data);
}

// 工作线程函数
void *worker_thread(void *args) {
    WorkerArgs *w_args = (WorkerArgs *)args;
    while (1) {
        int index;
        pthread_mutex_lock(w_args->mutex);
        if (*w_args->task_index >= w_args->num_tasks) {
            pthread_mutex_unlock(w_args->mutex);
            break;
        }
        index = (*w_args->task_index)++;
        pthread_mutex_unlock(w_args->mutex);

        Task *task = &w_args->tasks[index];
        if (task->input->device != 0) {
            transfer_data(task->input, 0);
        }
        task->func(task->input->data);
        if (task->output && task->output->device != 0) {
            transfer_data(task->output, 0);
        }
    }
    return NULL;
}

// 调度器：简单贪婪调度
void schedule_tasks(Task *tasks, int num_tasks, int num_workers) {
    pthread_t threads[num_workers];
    int task_index = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    WorkerArgs args = { tasks, num_tasks, &task_index, &mutex };

    // 创建工作线程
    for (int i = 0; i < num_workers; i++) {
        pthread_create(&threads[i], NULL, worker_thread, &args);
    }

    // 等待线程完成
    for (int i = 0; i < num_workers; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    // 初始化数据
    int input_data = 5;
    DataHandle input = { &input_data, sizeof(int), 1 };
    DataHandle output = { &input_data, sizeof(int), 0 };

    // 定义任务
    Task tasks[] = {
        { double_data, &input, &output, 1 }
    };
    int num_tasks = 1;

    // 调度任务
    printf("Starting StarPU-like scheduler...\n");
    schedule_tasks(tasks, num_tasks, 2); // 2 个工作线程

    printf("Final result: %d\n", input_data);
    return 0;
}
```

---

我调研完 starpu 之后有了新的想法，我可以用 RUST 或者 PYTHON 复现 Starpu，经过组内讨论后发现，RTOS 并不在乎内存的安全性问题，所以 RUST 其实做了很多冗余的工作，且并没有很多的性能提升和开创性。而且 python 对于操作系统的设计操作并不是很底层，不能很好的操作文件系统，所以这这个想法最终被我们组否定。:sob:

:heart:

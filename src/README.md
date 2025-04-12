# README for sources

一共 2 个目录，`uinc`、`sys`。

其中，`uinc` 是放头文件 (`.h`) 的，`src` 是放源文件 (`.c`) 和系统内部头文件的。`uinc` 放用户可以使用的头文件，其中包括 API 的原型、一些常量、类型定义等等；它们的包含关系 (以 task 子系统为例)

```
用户代码.c
  |_________uinc/lightrt.h
              |________uinc/task.h           (用 _TASK_U_H 作为 ifndef)
                         |_____________
                                       |
sys/task.c (task subsystem impl.)      |
  |_________sys/task/task.h            |     (用 _TASK_H 作为 ifndef)
              |________uinc/task.h     |     (用 _TASK_U_H 作为 ifndef)
                         |_________sys/global.h
```
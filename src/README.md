# README for sources

```
用户代码.c   (usrc/main.c)
  |_________uinc/lightrt.h
              |________uinc/task.u.h         (用 _TASK_U_H 作为 ifndef)
                         |_____________
                                       |
sys/task.c (task subsystem impl.)      |
  |_________sys/task/task.h            |     (用 _TASK_H 作为 ifndef)
  |           |________uinc/task.h     |     (用 _TASK_U_H 作为 ifndef)
  |                      |_________sys/global.h
  |
  |_________sys/other/other.s.h              (其他子系统的 header)
              |________uinc/other.u.h
                         |_________...

```

## Coding rules

-   每个子系统 (假设名称是 X) 只能包括：
    -   `sys/X/X.c` -- 主要的实现
    -   `sys/X/X.h` -- 主要的定义
    -   `sys/X/X.s.h` -- 其他子系统需要使用这个子系统时的头文件
    -   `sys/X/X...` 的其他内部文件 (.c, .h, 等等)
    -   `uinc/X/X.u.h` -- 用户需要使用这个子系统时候的头文件
    -   `makefile` 里面对应的项目
-   需要展示给用户的东西写在 `*.u.h` 里面，内部的东西写在 `*.h` 里面，`*.h` 里面必须 include `*.u.h`
-   头文件中代码前必须使用 `#ifndef _<头文件名>_H` 和 `#define _<头文件名>_H`，代码后必须使用 `#endif`，禁止使用 `#pragma once`
-   返回 `int` 的用户 API 永远是 `STATUS_***` 的宏定义之一 (在 `global.h`)。优先使用已有的状态代码，如果需要新增，则加到尾部
-   未实现的函数永远只返回 `STATUS_NOT_IMPLEMENTED`，未实现的宏定义后面加上 NOTIMPL (只要使用这些宏定义必定导致编译失败)
-   所有常量 (除了数组下标开始为 `0` 这类非常显然的)，都必须使用位于 `<子系统名称>.h` 中定义的宏
-   常量宏定义必须使用全部大写的名称
-   全局列表 **\_ 的大小只能由 `MAX\_\_**\_ITEM`的宏定义，并使用`IDWRONG(sig, SIGNAL)` 这样的写法进行检查
-   对于用户传入的参数必须进行检查，对有问题的输入参数必须只返回 `STATUS_INVALID_PARAMETER`，不能做其他任何事情
-   禁止使用 `malloc` 和 `free`
-   禁止使用 C++ 功能
-   系统对象，即使只需要一个成员，也必须写成结构体
-   用户 API 前面必须有 `LRT_UAPI` (例如 `LRT_UAPI int exec(entryproc p, void* param, const startinf* si);`)
-   不允许直接操作其他子系统的对象和内部状态，必须征得另一个子系统作者同意后，使用对方提供的 `LRT_SAPI` 标记的系统 API，这些 API 用 `_` 开头
-   一般不使用枚举 (enum)

## Formatting

目前的代码格式化选项是：

-   C/C++: Microsoft C/C++ Extension
-   其他: 默认为 Prettier, tab=4, no tab

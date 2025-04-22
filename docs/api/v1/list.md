# System APIs

## Symbol definition 符号定义

-   ⚠️ = 内部 (unsafe) API

## Return values 返回值定义

### 整数返回的 API

-   `0` = 成功完成
-   负数 = 失败
-   正数 = 按 API 定义

### 指针返回的 API

-   `NULL` = `0` = 失败

## Process control 任务控制

所有需要传入 PID 的都可以传入 `-1` 表示当前的 PID。

-   `int exec(entryproc p, void* param, const startinf* si)` = 启动新的任务

-   `int terminate(int pid)` = 中止任务

-   `int cpid(int* pid)` = 获取当前 PID

-   `int pinfo(int pid, procinf* pi)` = 通过 PID 得到任务信息 (复制到用户区)

    -   `int suspend(int pid, int s)` = 挂起 (s = 1) 或者正常运行 (s = 0) 一个任务
    -   `int rtime(int pid, int s, int timeout)` = (s = 1) 在 timeout 或者 `realtime(pid, 0, X)` 前不要调度走

-   `int pset(int pid, const procinf* pi)` = 设置 PID 对应的任务信息 (从用户区)

-   `int ppinfo(int pid, procinf** ppi)` ⚠️= 访问系统内部的任务信息

-   `int swtch(int pid)` = 调度到 PID

-   `int sleep(int ms)` = 等待至少 ms 后调度，传入 0 表示 yield()，传入 -1 表示无限等待

-   `int sleepus(int us)` = 等待至少 us 后调度，传入 0 表示 yield()，传入 -1 表示无限等待

-   `int brkwait(int pid)` = 使得 PID 退出等待，例如 `sleep()` 带来的等待

-   `int yield()` = 调度走

## Signal & Synchronization, Access control 信号和同步、访问控制

-   `int signew(int flag, int* sig)` = 创建信号，可以创建全局 (所有处理器共享) 或者局部 (处理器内或者任务内)

-   `int sigclose(int sig)` = 关闭信号

-   `int sigset(int sig, int s)` = 设置 (s = 1) 或恢复 (s = 0) 信号

-   `int waitsig(int sig, int timeout)` = 等待信号 (timeout = -1 表示一直等待)

-   `int waitany(const int* sigs, int nsig, int timeout)` = 等待任意一个信号 (timeout = -1 表示一直等待)

-   `int readsig(int sig)` = 读取信号状态

## Environment 环境

-   `int cuid(int* uid)` = 获取当前处理器 ID

-   `int sysinfo(sysinf* info)` = 获取系统信息

-   `int now(tick *t);` = 获取 tick 计数器的值

-   `int timeadd(tick *t, timespan *span);` = 时间相加

-   `int timetospan(tick *t, timespan *span);` = 时间转换为 span

-   `int timesub(tick *ta, tick *tb, timespan *span);` = 时间相减

-   `int timeneg(timespan *t);` = 时间段取相反数

-   `int timetest(timespan *t, int *result);` = 判断 timespan 是正、负还是零

## Communication 数据包通信 (UDP like)

-   `int comsend(int uid, int port, void* buf, int len)` 发送 [buf, buf + len) 到目标

-   `int comrecv(int port, void* buf, int len, int* wlen)` 阻塞式接收

-   `int comcheck(int uid)` 检查连接是否正常

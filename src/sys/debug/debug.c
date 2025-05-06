/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

debug.c - debug information implementation

*/

#include "debug.h"
#include "../../uinc/timer.u.h"
#include <stdbool.h>
// 暂时借用<stdio.h>来实现输出
#include <stdio.h>

// system log buffer
sys_log_info sys_log_list[MAX_LOG_ITEM];
int sys_log_index = 0;

// user log buffer
log_info log_list[MAX_LOG_ITEM];
int user_log_index = 0;

// 内存统计
mem_info memory_stat = {0};

// 性能统计
perf_info perf_stat = {0};

// init debug system
int _debug_init(void)
{
    // initialize default log level
    log_level = LOG_LEVEL_INFO;
    sys_log_level = LOG_LEVEL_FATAL;

    // initialize log buffers
    sys_log_index = 0;
    user_log_index = 0;

    // 初始化内存统计
    memory_stat.total_mem = 96 * 1024; // 总内存为96KB
    memory_stat.used_mem = 0;

    // 性能统计在定义时已初始化过

    // 初始化时间戳
    start_time = 0;
    stop_time = 0;

    // 记录系统初始化完成
    _log(LOG_LEVEL_INFO, "Debug system initialized.\n");

    return STATUS_SUCCESS;
}

// User API: set log level
int log_set(int8_t level)
{
    if (level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_OFF)
    {
        return STATUS_INVALID_PARAMETER;
    }

    log_level = level;
    return STATUS_SUCCESS;
}

// User API: get log level
int log_get(int8_t *level)
{
    if (UPTRWRONG(level))
    {
        return STATUS_INVALID_PARAMETER;
    }

    *level = log_level;
    return STATUS_SUCCESS;
}

// System API: log message
int _log(int8_t level, const char *msg)
{
    if (level < sys_log_level)
    {
        return STATUS_SUCCESS; // 低于系统日志级别的消息不输出
    }

    // 获取当前时间作为时间戳
    tick current_time;
    now(&current_time);

    // 保存到系统日志缓冲区
    sys_log_list[sys_log_index].level = level;
    sys_log_list[sys_log_index].message = msg;

    // 更新日志索引，循环使用缓冲区
    sys_log_index = (sys_log_index + 1) % MAX_LOG_ITEM;

    // 日志级别前缀
    const char *level_prefix[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

    // 暂时输出到控制台
    printf("[%llu][SYS]/*[%s]*/ %s\n", current_time, level_prefix[level], sys_log_list[sys_log_index].message);

    // 如果是致命错误，需要系统另外处理
    if (level == LOG_LEVEL_FATAL)
    {
        // TODO: 处理致命错误
    }

    return STATUS_NOT_IMPLEMENTED;
}

// User API: log message
int log(int8_t level, const char *msg)
{
    if (level < log_level)
    {
        return STATUS_SUCCESS; // 低于用户日志级别的消息不输出
    }

    if (UPTRWRONG(msg))
    {
        return STATUS_INVALID_PARAMETER;
    }

    // 获取当前时间作为时间戳
    tick current_time;
    now(&current_time);

    // 保存到用户日志缓冲区
    log_list[sys_log_index].level = level;
    log_list[sys_log_index].message = msg;

    // 更新日志索引，循环使用缓冲区
    user_log_index = (user_log_index + 1) % MAX_LOG_ITEM;

    // 日志级别前缀
    const char *level_prefix[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

    // 暂时输出到控制台
    printf("[%llu][USR]/*[%s]*/ %s\n", current_time, level_prefix[level], log_list[user_log_index].message);

    // 如果是致命错误，需要将控制权交还给系统处理
    if (level == LOG_LEVEL_FATAL)
    {
        // TODO: 处理致命错误
    }

    return STATUS_NOT_IMPLEMENTED;
}

// System API: assert function
void _assert(int condition, const char *msg)
{
    if (!condition)
    {
        _log(LOG_LEVEL_FATAL, msg);
        // 抛出问题
    }
}

// User API: get memory statistics
int memstat(int *total, int *used)
{
    if (UPTRWRONG(total) || UPTRWRONG(used))
    {
        return STATUS_INVALID_PARAMETER;
    }

    *total = memory_stat.total_mem;
    *used = memory_stat.used_mem;

    return STATUS_SUCCESS;
}

// 内部函数: 更新内存使用情况
int _update_memory_stat(int delta)
{
    if (memory_stat.used_mem + delta < 0)
    {
        return STATUS_INVALID_PARAMETER; // 不能为负
    }
    if (memory_stat.used_mem + delta > memory_stat.total_mem)
    {
        return STATUS_RESOURCE_INSUFFICIENT; // 内存溢出
    }

    memory_stat.used_mem += delta;
    return STATUS_SUCCESS;
}

// User API: start timer
int timestart(void)
{
    now(&start_time);
    return STATUS_SUCCESS;
}

// User API: stop timer and get elapsed time
int timestop(timespan *ts)
{
    if (UPTRWRONG(ts))
    {
        return STATUS_INVALID_PARAMETER;
    }

    now(&stop_time);

    *ts = (timespan)stop_time - (timespan)start_time;

    return STATUS_SUCCESS;
}

// User API: get performance statistics
int perfstat(int pid, perf_info *info)
{
    if (UPTRWRONG(info))
    {
        return STATUS_INVALID_PARAMETER;
    }

    // 填充性能统计信息
    info->idle_time = perf_stat.idle_time;
    info->busy_time = perf_stat.busy_time;
    info->loop_count = perf_stat.loop_count;
    info->interrupt_count = perf_stat.interrupt_count;
    info->max_task_time = perf_stat.max_task_time;

    // 计算有关数据
    uint8_t idle_percent = info->idle_time * 100 / (info->idle_time + info->busy_time);

    // 暂用printf输出
    printf("Idle Time: %llu, Busy Time: %llu, Idle Percent: %hhu, Loop Count: %u, Interrupt Count: %u, Max Task Time: %u\n", info->idle_time, info->busy_time, idle_percent, info->loop_count, info->interrupt_count, info->max_task_time);

    return STATUS_NOT_IMPLEMENTED;
}

// 内部函数: 更新性能统计
int _update_perf_stat(bool is_idle, timespan time_spent)
{
    // 更新性能统计
    perf_stat.loop_count++;
    perf_stat.interrupt_count++;

    // 更新空闲时间和忙碌时间
    if (is_idle)
    {
        perf_stat.idle_time += time_spent;
    }
    else
    {
        perf_stat.busy_time += time_spent;
        if (time_spent > perf_stat.max_task_time)
        {
            perf_stat.max_task_time = time_spent;
        }
    }

    return STATUS_SUCCESS;
}

#include <stdarg.h>

// Helper function to print a number in the specified base
static int print_number(unsigned int num, int base, int is_signed, int *count)
{
    char buf[32]; // Enough for 32-bit numbers in binary
    int len = 0;
    unsigned int n = num;

    // Handle negative numbers for any base when signed
    if (is_signed && (int)num < 0)
    {
        dputchar('-');
        n = -(int)num;
        (*count)++;
    }

    // Convert number to string in given base
    do
    {
        int digit = n % base;
        buf[len++] = digit < 10 ? '0' + digit : 'a' + (digit - 10);
        n /= base;
    } while (n);

    // Output digits in reverse order
    while (len--)
    {
        dputchar(buf[len]);
        (*count)++;
    }

    return *count;
}

int dprintf(const char *fmt, ...)
{
    va_list args;
    int count = 0;
    char c, *s;
    int i;

    va_start(args, fmt);
    while ((c = *fmt++))
    {
        if (c != '%')
        {
            dputchar(c);
            count++;
            continue;
        }
        c = *fmt++;
        switch (c)
        {
        case 'd':
            i = va_arg(args, int);
            print_number((unsigned int)i, 10, 1, &count);
            break;
        case 'u':
            i = va_arg(args, unsigned int);
            print_number(i, 10, 0, &count);
            break;
        case 'x':
            i = va_arg(args, unsigned int);
            print_number(i, 16, 1, &count);
            break;
        case 's':
            s = va_arg(args, char *);
            while (*s)
            {
                dputchar(*s++);
                count++;
            }
            break;
        case 'c':
            dputchar((char)va_arg(args, int));
            count++;
            break;
        default:
            dputchar(c);
            count++;
            break;
        }
    }
    va_end(args);
    return count;
}

// // User API: get task debug information
// int taskinfo(int pid, task_debug_info *info)
// {
//     if (UPTRWRONG(info))
//     {
//         return STATUS_INVALID_PARAMETER;
//     }

//     // TODO

//     return STATUS_SUCCESS;
// }

// // User API: get task list
// int tasklist(task_debug_info *info, int max)
// {
//     if (UPTRWRONG(info) || max <= 0)
//     {
//         return STATUS_INVALID_PARAMETER;
//     }

//     // TODO
//
//     return STATUS_NOT_IMPLEMENTED;
// }

// // User API: get communication debug information
// int comminfo(int uid, int pid, comm_debug_info *info)
// {
//     if (UPTRWRONG(info))
//     {
//         return STATUS_INVALID_PARAMETER;
//     }

//     // 填充通信调试信息
//     info->uid = uid;
//     info->pid = pid;
//     info->messages_sent = 0;
//     info->messages_received = 0;
//     info->bytes_sent = 0;
//     info->bytes_received = 0;
//     info->errors = 0;

//     return STATUS_SUCCESS;
// }

// // User API: set IO trace
// int iotrace(int port, int enable)
// {
//     // TODO
//
//     return STATUS_NOT_IMPLEMENTED;
// }

// // User API: get IO status information
// int iostat(int port, io_debug_info *info)
// {
//     if (UPTRWRONG(info))
//     {
//         return STATUS_INVALID_PARAMETER;
//     }

//     // 填充IO状态信息
//     info->port = port;
//     info->read_count = 0;
//     info->write_count = 0;
//     info->error_count = 0;
//     info->last_operation = 0;

//     return STATUS_SUCCESS;
// }

// // User API: get bus status information
// int busstat(bus_debug_info *info)
// {
//     if (UPTRWRONG(info))
//     {
//         return STATUS_INVALID_PARAMETER;
//     }

//     // 填充总线状态信息
//     info->bus_utilization = 0;
//     info->collision_count = 0;
//     info->error_rate = 0;

//     return STATUS_SUCCESS;
// }

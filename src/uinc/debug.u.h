/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

debug.u.h - User header for debug information

*/

#ifndef _DEBUG_U_H
#define _DEBUG_U_H

#include "../sys/global.h"
#include "timer.u.h"
#include <stdint.h>

// log level definition
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4
#define LOG_LEVEL_OFF 5
// Max number of log items
#define MAX_LOG_ITEM 100
// Max length of log message
#define MAX_LOG_MSG 256

// log structure for user
typedef struct
{
    int8_t level;              // log level
    char message[MAX_LOG_MSG]; // log message
} log_info;

// log list for user
extern log_info log_list[MAX_LOG_ITEM];

// user log level(default: LOG_LEVEL_INFO)
extern int8_t log_level; // could be changed by logset()

// output log api
LRT_UAPI int log_set(int8_t level);              // set log level
LRT_UAPI int log_get(int8_t *level);             // get log level
LRT_UAPI int log(int8_t level, const char *msg); // write log message

// memory statatics structure & api
typedef struct
{
    int total_mem; // total memory size
    int used_mem;  // used memory size
} mem_info;

LRT_UAPI int memstat(int *total, int *used); // get memory statistics

// performance statistics structure & api
typedef struct
{
    timespan idle_time; // idle time
    timespan busy_time; // busy time
    uint32_t loop_count; // number of loop iterations
    uint32_t interrupt_count; // number of interrupts
    uint32_t max_task_time;   // maximum task time
} perf_info;

LRT_UAPI int perfstat(int pid, perf_info *info); // get performance statistics

// timer api
extern tick start_time;
extern tick stop_time;
LRT_UAPI int timestart();                  // start timer
LRT_UAPI int timestop(timespan *timespan); // return elapsed time

// // task debug structure & api
// typedef struct
// {
//     int pid;            // task id
//     int nice;           // task nice value
//     int status;         // task status
//     int waittype;       // task wait type
//     uint32_t mem_usage; // memory usage
// } task_debug_info;

// LRT_UAPI int taskinfo(int pid, task_debug_info *info); // get task debug info
// LRT_UAPI int tasklist(task_debug_info *info, int max); // get task list

// // communication debug structure & api
// typedef struct
// {
//     int uid;                    // processor id
//     int pid;                    // process id
//     uint32_t messages_sent;     // number of messages sent
//     uint32_t messages_received; // number of messages received
//     uint32_t bytes_sent;        // number of bytes sent
//     uint32_t bytes_received;    // number of bytes received
//     uint32_t errors;            // number of errors
// } comm_debug_info;

// LRT_UAPI int comminfo(int uid, int pid, comm_debug_info *info); // get communication debug info

// // IO debug structure & api
// typedef struct
// {
//     int port;                // port number
//     uint32_t read_count;     // count of read operations
//     uint32_t write_count;    // count of write operations
//     uint32_t error_count;    // count of errors
//     uint32_t last_operation; // last operation (read/write)
// } io_debug_info;

// LRT_UAPI int iotrace(int port, int enable);         // enable/disable IO trace
// LRT_UAPI int iostat(int port, io_debug_info *info); // get IO status

// // bus debug structure & api
// typedef struct
// {
//     uint32_t bus_utilization; // bus utilization (percentage)
//     uint32_t collision_count; // number of collisions
//     uint32_t error_rate;      // error rate (percentage)
// } bus_debug_info;

// LRT_UAPI int busstat(bus_debug_info *info); // get bus status

#endif

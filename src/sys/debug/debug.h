/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

debug.h - Internal header for debug information

*/

#ifndef _DEBUG_H
#define _DEBUG_H

#include "../global.h"
#include "../../uinc/debug.u.h"

// log structure for system
typedef struct
{
    int8_t level;              // log level
    char message[MAX_LOG_MSG]; // log message
} sys_log_info;

// log list for system
extern sys_log_info sys_log_list[MAX_LOG_ITEM];

// System log level(default: LOG_LEVEL_FATAL)
extern int8_t sys_log_level; // could be changed by logset()

// system api
LRT_SAPI int _debug_init(); // initialize debug system

LRT_SAPI int _log(int8_t level, const char *msg);      // system log message
LRT_SAPI int _update_memory_stat(int delta);           // update memory statistics
LRT_SAPI int _update_perf_stat(bool is_idle, timespan time_spent); // update performance statistics
LRT_SAPI void _assert(int condition, const char *msg); // assert function

#endif

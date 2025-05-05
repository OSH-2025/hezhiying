/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

debug.h - Internal header for debug information

*/

#ifndef _DEBUG_H
#define _DEBUG_H

#include "../global.h"
#include "../../uinc/debug.u.h"

// System log level(default: LOG_LEVEL_FATAL)
extern uint8_t sys_log_level; // could be changed by logset()

#ifdef DEBUG_ENABLED
#define ASSERT(condition, msg)                                  \
    if (!(condition))                                           \
    {                                                           \
        logmsg(LOG_LEVEL_FATAL, "Assertion failed: %s\n", msg); \
        while (1)                                               \
        {                                                       \
        } /* Infinite loop to halt execution */                 \
    }
#else
#define ASSERT(condition, msg) ((void)0) // No-op if DEBUG_ENABLED is not defined
#endif

LRT_SAPI void _debug_init();                              // Initialize debug system
LRT_SAPI int _debug_log(int level, const char *fmt, ...); // Log message
LRT_SAPI void _assert(int condition, const char *msg);     // Assert function

#endif

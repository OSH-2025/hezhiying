/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

task.h - Internal header for task manipulation

*/

#ifndef _TASK_U_H
#define _TASK_U_H

#include "../inc/global.h"

// entryproc: Entry proc for task
typedef int (*entryproc)(void* param);

// STARTUP_*: Startup flag constants. Should be bit-or'ed.
#define STARTUP_NORMAL       0
#define STARTUP_SUSPEND      MAKEBIT(1) // Startup but not executing any code until resume.
#define STARTUP_REPLACE      MAKEBIT(2) NOTIMPL // Replacing the current task.
#define STARTUP_FORK         MAKEBIT(3) NOTIMPL // Dulipicate the current task, like fork().

// startinf: Startup information
typedef struct
{
    uint flag;
} startinf;

// Execute a new task
LRT_UAPI int exec(entryproc p, void* param, const startinf* si);

#endif

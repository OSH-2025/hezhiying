/*

LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

task.h - Internal header for task manipulation

*/

#ifndef _TASK_H
#define _TASK_H

#include "../global.h"
#include "../../uinc/task.u.h"

#include <stdint.h>

typedef struct {
    // Manually saved
    uint r4;
    uint r5;
    uint r6;
    uint r7;
    uint r8;
    uint r9;
    uint r10;
    uint r11;

    // Automatically saved on exception
    uint r0;
    uint r1;
    uint r2;
    uint r3;
    uint r12;
    uint lr;
    uint pc;
    uint xpsr;

    // Additional state
    uint psp;
    uint control;
} context;

#define WAIT_NOWAIT    0
#define WAIT_SLEEP     1
#define WAIT_SIGNAL    2
#define WAIT_SIGNALS   3 NOTIMPL

typedef struct
{
    // Task ID, -1 = Not used
    int pid;

    // External SRAM bank
    uint rambank;

    // Context for multi-tasking
    context ctx;

    // Priority / nice value
    int nice; // Nice value; (less => higher priority), -1 = SUSPEND

    // Realtime mode
    int realtime; // In realtime mode? (1 = realtime)
    uint rtlow, rthigh; // Realtime timeout tick
    
    // Wait
    struct
    {
        int type; // Waiting type

        union
        {
            struct
            {
                uint wakeuplow, wakeuphigh; // Expected wakeup time
            } sleep;

            struct
            {
                int signal;
            } signal;
            
            struct
            {
                int* signals;
                int nsignal;
            } signals;
        } param;
    } wait;
} task;

extern task tsklist[MAX_TASK_ITEM];

#endif

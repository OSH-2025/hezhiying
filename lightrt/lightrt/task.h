#ifndef _TASK_H
#define _TASK_H

#include "lightrt.h"

typedef void (*task_routine)(void);

#define MAX_TASKS 16
#define STACK_SIZE 256

typedef struct
{
    uint32_t *sp;
    uint32_t stack[STACK_SIZE];

    int state;
    int prio;

#include "signal.waitable.h"
#include "timing.waitable.h"
} tcb_t;

void task_create(task_routine task, int id);
__attribute__((naked)) void task_start_first(uint32_t *sp);
__attribute__((naked)) void task_swtch_impl(tcb_t *next);
void task_swtch(int nextid);

#define TASK_EMPTY 0
#define TASK_NORMAL 1
#define TASK_SUSPEND 2
#define TASK_ABORT 3
#define TASK_WAIT 4

extern tcb_t *current_task;
extern int current_task_id;
extern tcb_t tasks[MAX_TASKS];

extern uint32_t tick;

#endif
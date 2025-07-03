#include "../lightrt.h"

#ifndef _TASK_H
#define _TASK_H

typedef void (*task_routine)(void *);

#define MAX_TASKS 4
#define STACK_SIZE 256

#define TASK_EMPTY 0
#define TASK_NORMAL 1
#define TASK_SUSPEND 2
#define TASK_ABORT 3
#define TASK_WAIT 4

#define WAIT_NONE 0
#define WAIT_SIGNAL 1
#define WAIT_TIMING 2
#define WAIT_DMA 3
#define WAIT_UART 4

typedef struct
{
    uint32_t *sp;
    uint32_t stack[STACK_SIZE];

    int state;
    int prio;

    struct
    {
        int which;
#include "task.wait.h"
    } wait_area;

    struct
    {
#include "task.tls.h"
    } tls_area; // The thread local storage area
} tcb_t;

void task_create(task_routine task, void *param, int id);
__attribute__((naked)) void task_start_first(uint32_t *sp);
__attribute__((naked)) void task_swtch_impl(tcb_t *next);
void task_swtch(int nextid);

extern tcb_t *current_task;
extern int current_task_id;
extern tcb_t tasks[MAX_TASKS];

extern uint32_t tick;

#endif
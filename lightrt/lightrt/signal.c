#include "signal.h"

signal_t signals[MAX_SIGNALS];

void signal_new(int id)
{
    signals[id].state = SIGNAL_INACTIVE;
}

void signal_active(int id)
{
    signals[id].state = SIGNAL_ACTIVE;

    for (int i = 0; i < MAX_TASKS; ++i)
    {
        if (tasks[i].state == TASK_WAIT && tasks[i].wait_signal_id == id)
        {
            // uart_printf("Resuming %d\n", i);
            tasks[i].state = TASK_NORMAL;
            tasks[i].wait_signal_id = -1;
        }
    }
}

void signal_inactive(int id)
{
    signals[id].state = SIGNAL_INACTIVE;
}

void signal_dispose(int id)
{
    signals[id].state = SIGNAL_EMPTY;
}

int signal_find_empty()
{
    for (int i = 0; i < MAX_SIGNALS; ++i)
    {
        if (signals[i].state == SIGNAL_EMPTY)
        {
            return i;
        }
    }
    return -1;
}

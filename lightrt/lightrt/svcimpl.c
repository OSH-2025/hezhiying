#include "lightrt.h"

uint32_t svc_uart_write(const char *message)
{
    uart_write(message);

    return E_SUCCESS;
}

uint32_t svc_task_start(task_routine routine, int id)
{
    if (id == -1)
    {
        for (int i = 0; i < MAX_TASKS; ++i)
        {
            if (tasks[i].state == TASK_EMPTY)
            {
                id = i;
                break;
            }
        }

        if (id == -1)
        {
            return E_INSUFFICIENT;
        }
    }

    if (tasks[id].state != TASK_EMPTY)
    {
        return E_OCCUPIED;
    }

    // uart_printf("Run %x => %d\n", routine, id);
    task_create(routine, id);

    return E_SUCCESS;
}

uint32_t svc_task_terminate(int id)
{
    if (id == -1)
    {
        id = current_task_id;
    }

    if (tasks[id].state == TASK_EMPTY)
    {
        return E_INVALID;
    }

    tasks[id].state = TASK_EMPTY;
    tasks[id].sp = 0;

    for (int i = 0; i < STACK_SIZE; ++i)
    {
        tasks[id].stack[i] = 0;
    }

    int next_id = -1;
    for (int i = 0; i < MAX_TASKS; ++i)
    {
        if (tasks[i].state == TASK_NORMAL)
        {
            next_id = i;
        }
    }

    task_swtch(next_id);

    // Actually will not return
    return E_SUCCESS;
}

uint32_t svc_task_yield(int nextid)
{
    // uart_printf("Yielding: %d\n", nextid);

    if (nextid == -1)
    {
        for (int i = 0; i < MAX_TASKS; ++i)
        {
            if (tasks[i].state == TASK_NORMAL)
            {
                nextid = i;
            }
        }

        if (nextid == -1)
        {
            return E_SINGULAR;
        }
    }
    else
    {
        if (tasks[nextid].state != TASK_NORMAL)
        {
            return E_INVALID;
        }
    }

    task_swtch(nextid);

    return E_SUCCESS;
}

uint32_t svc_task_state(int id, int state)
{
    if (id == -1)
    {
        id = current_task_id;
    }

    if (tasks[id].state == TASK_EMPTY)
    {
        return E_INVALID;
    }

    if (state == TASK_EMPTY)
    {
        return E_INVALID;
    }

    tasks[id].state = state;

    return E_SUCCESS;
}

uint32_t svc_task_prio(int id, int prio)
{
    if (id == -1)
    {
        id = current_task_id;
    }

    if (tasks[id].state == TASK_EMPTY)
    {
        return E_INVALID;
    }

    tasks[id].prio = prio;

    return E_SUCCESS;
}

uint32_t svc_clock_tick()
{
    return tick;
}

uint32_t svc_task_current()
{
    return current_task_id;
}

uint32_t svc_signal_new(int *id)
{
    int new_signal = signal_find_empty();

    if (new_signal == -1)
    {
        return E_INSUFFICIENT;
    }

    signals[new_signal].state = SIGNAL_INACTIVE;
    *id = new_signal;

    return E_SUCCESS;
}

uint32_t svc_signal_active(int id)
{
    if (signals[id].state == SIGNAL_EMPTY)
    {
        return E_INVALID;
    }

    signal_active(id);

    return E_SUCCESS;
}

uint32_t svc_signal_inactive(int id)
{
    if (signals[id].state == SIGNAL_EMPTY)
    {
        return E_INVALID;
    }

    signal_inactive(id);

    return E_SUCCESS;
}

uint32_t svc_signal_dispose(int id)
{
    if (signals[id].state == SIGNAL_EMPTY)
    {
        return E_INVALID;
    }

    signal_dispose(id);

    return E_SUCCESS;
}

uint32_t svc_signal_wait(int id)
{
    uart_printf("Wait signal %d\n", id);

    if (signals[id].state == SIGNAL_EMPTY)
    {
        return E_INVALID;
    }

    tasks[current_task_id].wait_signal_id = id;
    tasks[current_task_id].state = TASK_WAIT;

    uart_printf("Current task %d set to waiting signal %d (state %d)\n", current_task_id, id, tasks[current_task_id].state);

    task_swtch(0);

    return E_SUCCESS;
}

uint32_t svc_signal_read(int id)
{
    return signals[id].state;
}

uint32_t svc_clock_delay(int wait_tick)
{
    // uart_printf("Delay %d.\n", wait_tick);

    tasks[current_task_id].wait_due_time = tick + wait_tick;
    tasks[current_task_id].state = TASK_WAIT;

    task_swtch(0);

    return E_SUCCESS;
}

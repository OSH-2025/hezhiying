#include "../lightrt.h"

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

    tasks[current_task_id].wait_area.wait_signal_id = id;
    tasks[current_task_id].state = TASK_WAIT;
    tasks[current_task_id].wait_area.which = WAIT_SIGNAL;

    uart_printf("Current task %d set to waiting signal %d (state %d)\n", current_task_id, id, tasks[current_task_id].state);

    task_swtch(-1);

    return E_SUCCESS;
}

uint32_t svc_signal_read(int id)
{
    return signals[id].state;
}
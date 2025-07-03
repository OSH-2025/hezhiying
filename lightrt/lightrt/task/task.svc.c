#include "../lightrt.h"

uint32_t svc_task_start(task_routine routine, void *param, int id)
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

    // uart_printf("Run %x => %d, param %x\n", routine, id, param);
    task_create(routine, param, id);

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

uint32_t svc_task_current()
{
    return current_task_id;
}

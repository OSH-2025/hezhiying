#include "../lightrt.h"

uint32_t svc_clock_tick()
{
    return tick;
}

uint32_t svc_clock_delay(int wait_tick)
{
    // uart_printf("Delay %d.\n", wait_tick);

    tasks[current_task_id].wait_area.wait_due_time = tick + wait_tick;
    tasks[current_task_id].state = TASK_WAIT;
    tasks[current_task_id].wait_area.which = WAIT_TIMING;

    task_swtch(-1);

    return E_SUCCESS;
}

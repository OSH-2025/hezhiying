#include "../lightrt.h"

int uart_wait_taskid = -1;

uint32_t svc_uart_write(const char *message)
{
    uart_write(message);

    return E_SUCCESS;
}

uint32_t svc_uart_read(char *buf, int size)
{
    if (uart_wait_taskid != -1)
    {
        return E_OCCUPIED;
    }

    if (tasks[uart_wait_taskid].state == TASK_WAIT)
    {
        return E_INVALID;
    }

    current_task->state = TASK_WAIT;
    current_task->wait_area.which = WAIT_UART;
    current_task->wait_area.wait_uart_pbuf = buf;
    current_task->wait_area.wait_uart_ebuf = buf + size;

    uart_wait_taskid = current_task_id;

    uart_printf("Current task %d set to waiting UART\n", current_task_id);

    task_swtch(-1);
    return E_SUCCESS;
}

#include "../lightrt.h"

int dma_owner_task = -1;

uint32_t svc_dma_init()
{
    // uart_printf("DMA Initializing owner = %d.\n", dma_owner_task);
    if (dma_owner_task != -1)
    {
        if (dma_owner_task == current_task_id && !dma_status())
            return E_BUSY;
        return E_OCCUPIED;
    }

    dma_owner_task = current_task_id;
    dma_init();

    // uart_printf("DMA Initialized.\n");

    return E_SUCCESS;
}

#define dma_check_return                       \
    do                                         \
    {                                          \
        if (dma_owner_task == -1)              \
            return E_NOTREADY;                 \
        if (dma_owner_task != current_task_id) \
            return E_OCCUPIED;                 \
    } while (0)

uint32_t svc_dma_1byte(void *src, void *dst, int cblk)
{
    dma_check_return;

    dma_config(src, dst, cblk, 1);
    return E_SUCCESS;
}

uint32_t svc_dma_2byte(void *src, void *dst, int cblk)
{
    dma_check_return;

    dma_config(src, dst, cblk, 2);
    return E_SUCCESS;
}

uint32_t svc_dma_4byte(void *src, void *dst, int cblk)
{
    dma_check_return;

    dma_config(src, dst, cblk, 4);
    return E_SUCCESS;
}

uint32_t svc_dma_start()
{
    dma_check_return;

    uart_printf("DMA started.\n");

    dma_start();
    return E_SUCCESS;
}

uint32_t svc_dma_busy()
{
    return dma_status() != 0 ? E_SUCCESS : E_BUSY;
}

uint32_t svc_dma_dispose()
{
    dma_dispose();
    dma_owner_task = -1;
    return E_SUCCESS;
}

uint32_t svc_dma_wait()
{
    tasks[current_task_id].state = TASK_WAIT;
    tasks[current_task_id].wait_area.which = WAIT_DMA;

    task_swtch(-1);

    return E_SUCCESS;
}

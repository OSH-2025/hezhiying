/* This file is generated using SVC compiler. DO NOT DIRECTLY MODIFY. Edit svc.def file and run svcc instead. */
#include <stdint.h>
#include "svc.h"
void svcHandler(uint32_t *svc_args)
{
    uint8_t *svc_instr = (uint8_t *)(svc_args[6] - 2);
    uint8_t svc_number = *svc_instr & 0xFF;
    switch (svc_number)
    {
        case 10:
            svc_args[0] = svc_task_start(
                (task_routine)(svc_args[0]), 
                (void*)(svc_args[1]), 
                (int)(svc_args[2])
            );
            break;
        case 11:
            svc_args[0] = svc_task_terminate(
                (int)(svc_args[0])
            );
            break;
        case 12:
            svc_args[0] = svc_task_yield(
                (int)(svc_args[0])
            );
            break;
        case 13:
            svc_args[0] = svc_task_state(
                (int)(svc_args[0]), 
                (int)(svc_args[1])
            );
            break;
        case 14:
            svc_args[0] = svc_task_current(
            );
            break;
        case 15:
            svc_args[0] = svc_task_prio(
                (int)(svc_args[0]), 
                (int)(svc_args[1])
            );
            break;
        case 20:
            svc_args[0] = svc_clock_tick(
            );
            break;
        case 21:
            svc_args[0] = svc_clock_delay(
                (int)(svc_args[0])
            );
            break;
        case 30:
            svc_args[0] = svc_signal_new(
                (int*)(svc_args[0])
            );
            break;
        case 31:
            svc_args[0] = svc_signal_active(
                (int)(svc_args[0])
            );
            break;
        case 32:
            svc_args[0] = svc_signal_inactive(
                (int)(svc_args[0])
            );
            break;
        case 33:
            svc_args[0] = svc_signal_dispose(
                (int)(svc_args[0])
            );
            break;
        case 34:
            svc_args[0] = svc_signal_wait(
                (int)(svc_args[0])
            );
            break;
        case 35:
            svc_args[0] = svc_signal_read(
                (int)(svc_args[0])
            );
            break;
        case 70:
            svc_args[0] = svc_uart_write(
                (const char*)(svc_args[0])
            );
            break;
        case 71:
            svc_args[0] = svc_uart_read(
                (char*)(svc_args[0]), 
                (int)(svc_args[1])
            );
            break;
        case 80:
            svc_args[0] = svc_shmem_size(
            );
            break;
        case 81:
            svc_args[0] = svc_shmem_base(
            );
            break;
        case 82:
            svc_args[0] = svc_shmem_lock(
            );
            break;
        case 83:
            svc_args[0] = svc_shmem_unlock(
            );
            break;
        case 90:
            svc_args[0] = svc_rand_seed(
                (int)(svc_args[0])
            );
            break;
        case 91:
            svc_args[0] = svc_rand_next(
            );
            break;
        case 100:
            svc_args[0] = svc_gpio_led_set(
            );
            break;
        case 101:
            svc_args[0] = svc_gpio_led_init(
            );
            break;
        case 110:
            svc_args[0] = svc_dma_init(
            );
            break;
        case 111:
            svc_args[0] = svc_dma_1byte(
                (void*)(svc_args[0]), 
                (void*)(svc_args[1]), 
                (int)(svc_args[2])
            );
            break;
        case 112:
            svc_args[0] = svc_dma_2byte(
                (void*)(svc_args[0]), 
                (void*)(svc_args[1]), 
                (int)(svc_args[2])
            );
            break;
        case 113:
            svc_args[0] = svc_dma_4byte(
                (void*)(svc_args[0]), 
                (void*)(svc_args[1]), 
                (int)(svc_args[2])
            );
            break;
        case 114:
            svc_args[0] = svc_dma_start(
            );
            break;
        case 115:
            svc_args[0] = svc_dma_busy(
            );
            break;
        case 116:
            svc_args[0] = svc_dma_dispose(
            );
            break;
        case 117:
            svc_args[0] = svc_dma_wait(
            );
            break;
    }
}
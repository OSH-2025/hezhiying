#include "lightrt.h"
#include <stdarg.h>

#include "user/syscall.h"

int signalid;

void mytask(void)
{
    while (1)
    {
        // lrt_uart_write("Hello, world!\n");
        lrt_signal_wait(signalid);
        uart_printf("Hello, world! Task 1 Current time: %d\n", lrt_clock_tick());
    }
}

void mytask2(void)
{
    while (1)
    {
        lrt_clock_delay(20);
        // lrt_uart_write("Hello, world!\n");
        uart_printf("Hello, world! Task 2 Current time: %d\n", lrt_clock_tick());
        lrt_signal_active(signalid);
    }
}

// Task implementations
void init(void)
{
    uint32_t currentid = lrt_task_current();
    uart_printf("Init task started. Task ID is %d.\n", currentid);

    lrt_signal_new(&signalid);

    lrt_task_start(mytask, 1);
    lrt_task_prio(1, 2);

    lrt_task_start(mytask2, 2);
    lrt_task_prio(2, 1);

    // lrt_task_terminate(-1);

    while (1)
        ;
}

/*
     Stack frame:
     svc_args[0] = R0
     svc_args[1] = R1
     svc_args[2] = R2
     svc_args[3] = R3
     svc_args[4] = R12
     svc_args[5] = LR (return address)
     svc_args[6] = PC (where SVC occurred + 2)
     svc_args[7] = xPSR
    */

int main(void)
{
    volatile uint32_t *ptr = (volatile uint32_t *)0x60000000;
    *ptr = 0x12345678;

    // rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();
    uart_write("This is LightRT Kernel built " __DATE__ ", " __TIME__ "\n");

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(7200000); // 10ms @72MHz
    systick_interrupt_enable();
    systick_counter_enable();

    uart_write("Initialization completed.\n");

    uart_write("Starting init task...\n");

    task_create(init, 0);
    current_task = &tasks[0];
    register uint32_t *sp asm("r0") = current_task->sp;
    task_start_first(sp);

    while (1)
        __asm("wfi"); // Never reached
}

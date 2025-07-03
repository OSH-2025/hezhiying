#include "lightrt.h"
#include <stdarg.h>

#include "user/syscall.h"

int signalid;
mxsram volatile uint32_t sharedvar;

/* Code for feature test */
void panic()
{
    reset_handler();
}

void test_task1(void *arg)
{
    uart_printf("Task1 started. Argument = %x", arg);

    while (1)
    {
        lrt_uart_write("Hello, world!\n");
        lrt_signal_wait(signalid);
        uart_printf("Hello, world! Task 1 Current time: %d\n", lrt_clock_tick());
    }
}

void test_dma(void *arg)
{
    uint32_t src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint32_t dst[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    lrt_dma_init();
    lrt_dma_4byte(src, dst, 5);
    lrt_dma_start();

    lrt_dma_wait();

    lrt_dma_dispose();

    for (int i = 0; i < 10; ++i)
    {
        uart_printf("Copied %d -> %d\n", src[i], dst[i]);
    }
}

void test_uart()
{
    uart_printf("Testing UART print...\n");

    uart_printf("Testing UART read. Please press 10 keys.\n");
    char buf[10];
    lrt_uart_read(buf, 10);

    for (int i = 0; i < 10; ++i)
    {
        uart_printf("%c = %d\n", buf[i], buf[i]);
    }
}

void test_task0(void *arg)
{
    lrt_uart_write("Task 0 created.\n");

    while (1)
        ;
}

void test_task2(void *arg)
{
    while (1)
    {
        lrt_clock_delay(20);
        // lrt_uart_write("Hello, world!\n");
        uart_printf("Hello, world! Task 2 Current time: %d\n", lrt_clock_tick());
        lrt_signal_active(signalid);
    }
}

void test_exmem(void *arg)
{
    *maddr(0x60000008) = 1958;
    uint32_t read_value = *maddr(0x60000008);
    uart_printf("Read test: %d\n", read_value);

    uart_printf("exmem lock = %d\n", lrt_shmem_lock());
    *maddr(0x60000008) = 2025;
    read_value = *maddr(0x60000008);
    uart_printf("Read test 2: %d\n", read_value);
    uart_printf("exmem unlock = %d\n", lrt_shmem_unlock());

    uart_printf("svar is in %x\n", &sharedvar);
}

// Task implementations
void init(void *arg)
{
    uint32_t currentid = lrt_task_current();
    uart_printf("Init task started. Task ID is %d, param is %x.\n", currentid, arg);

    lrt_signal_new(&signalid);

    lrt_task_start(test_task1, 0, 1);
    lrt_task_prio(1, 2);

    uart_printf("Task 1 started.\n");

    lrt_task_start(test_task2, 0, 2);
    lrt_task_prio(2, 1);

    uart_printf("Task 2 started.\n");

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
    // rcc_clock_setup_in_hse_8mhz_out_72mhz();
    uart_setup();
    uart_write("This is LightRT Kernel built " __DATE__ ", " __TIME__ "\n");

    uart_write("  ,_,\n"
               " (.,.)\n"
               " (   )    LightRT\n"
               " -\"-\"-------------\n");

    uint32_t emembase = lrt_shmem_base();
    uint32_t ememsize = lrt_shmem_size();
    uart_printf("External shared memory controller reports: base %x size %x\n", emembase, ememsize);

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(7200000); // 10ms @72MHz
    systick_interrupt_enable();
    systick_counter_enable();

    uart_write("Initialization completed.\n");

    uart_write("Starting init task...\n");

    task_create(init, (void *)0x1958, 0);
    current_task = &tasks[0];
    register uint32_t *sp asm("r0") = current_task->sp;
    task_start_first(sp);

    while (1)
        __asm("wfi"); // Never reached
}

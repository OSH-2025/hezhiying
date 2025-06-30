#include "lightrt.h"
#include "task.h"

tcb_t tasks[MAX_TASKS];

#define YIELD_SYSTICK 0
#define YIELD_MANUAL 1
int yield_source;
int yield_manual_next;

uint32_t tick = 0;

void sys_tick_handler(void)
{
    ++tick;
    // uart_printf("SysTick: %d\n", tick);

    for (int i = 0; i < MAX_TASKS; ++i)
    {
        if (tasks[i].state == TASK_WAIT && tasks[i].wait_due_time <= tick)
        {
            uart_printf("Tick: resuming task %d\n", i);
            tasks[i].state = TASK_NORMAL;
            tasks[i].wait_due_time = -1;
        }
    }

    yield_source = YIELD_SYSTICK;
    SCB_ICSR |= SCB_ICSR_PENDSVSET; // Set PendSV to trigger
}

void task_create(task_routine task, int id)
{
    // uint32_t *stack_top = tasks[i].stack + STACK_SIZE - 16;

    uint32_t *sp = &tasks[id].stack[STACK_SIZE - 1];

    // Exception stack frame - 8 registers
    sp -= 8; // reserve space for R0-R3, R12, LR, PC, xPSR

    sp[0] = 0x00000000;     // R0
    sp[1] = 0x01010101;     // R1
    sp[2] = 0x02020202;     // R2
    sp[3] = 0x03030303;     // R3
    sp[4] = 0x12121212;     // R12
    sp[5] = 0xFFFFFFFD;     // LR (return to thread mode, use PSP)
    sp[6] = (uint32_t)task; // PC (start address)
    sp[7] = 0x01000000;     // xPSR (Thumb bit)

    // Save R4-R11 context (optional - 8 registers)
    sp -= 8;
    for (int i = 0; i < 8; i++)
        sp[i] = 0x00000000;

    tasks[id].sp = sp;
    tasks[id].state = TASK_NORMAL;
    tasks[id].wait_due_time = -1;
    tasks[id].wait_signal_id = -1;
    tasks[id].prio = 0;

    // After this the stack space is
    /*
    (Low address) R4 ... R11 R0 R1 R2 R3 ... PC xPSR (High address) (stack bottom)
                  ^
                  SP
    */
}

tcb_t *current_task = 0;
tcb_t *select_next(void);

__attribute__((naked)) void pend_sv_handler(void)
{
    __asm volatile(
        "MRS R0, PSP\n" // Load current process stack pointer into R0

        // Save R4-R11 onto current task's stack
        "STMDB R0!, {R4-R11}\n"

        // Save the updated stack pointer into current_task->sp
        "LDR R1, =current_task\n"
        "LDR R2, [R1]\n"
        "STR R0, [R2]\n"

        // Call select_next to pick next task
        "BL select_next\n"

        // Save return value (next task pointer) into current_task
        "LDR R1, =current_task\n"
        "STR R0, [R1]\n"

        // Load the stack pointer of the new task
        "LDR R2, [R0]\n"        // R2 = new task->sp
        "LDMIA R2!, {R4-R11}\n" // Restore R4-R11 from new stack
        "MSR PSP, R2\n"         // Update PSP to point to new stack position

        "MOV R0, #3\n" // Use unpriv + PSP
        "MSR CONTROL, R0\n"
        "ISB\n"

        "LDR R0, =0xFFFFFFFD\n"
        "MOV LR, R0\n"

        "BX LR\n" // Return from exception, resumes the task
    );
}

int current_task_id = 0;

int get_next()
{
    int next_task = current_task_id;
    int t = next_task;
    do
    {
        // uart_printf("Trying task %d: state %d prio %d\n", t, tasks[t].state, tasks[t].prio);
        t = (t + 1) % MAX_TASKS;
        if (tasks[t].state == TASK_NORMAL && tasks[t].prio > tasks[next_task].prio)
        {
            // uart_printf("Selected task %d: state %d prio %d\n", t, tasks[t].state, tasks[t].prio);
            next_task = t;
        }
    } while (t != current_task_id);

    return next_task;
}

tcb_t *select_next(void)
{
    // uart_write("Enter select_next\n");

    int next_task = current_task_id;
    if (yield_source == YIELD_SYSTICK)
    {
        next_task = get_next();
        uart_printf("Select %d as next task.\n", next_task);
    }
    else
    {
        next_task = yield_manual_next;
        uart_printf("Select %d manually as next task.\n", next_task);
    }

    // uart_printf("Switch from task %d to %d, the new task has state %d.\n", current_task_id, next_task, tasks[next_task].state);
    current_task_id = next_task;
    return &tasks[current_task_id];
}

__attribute__((naked)) void task_start_first(uint32_t *sp)
{
    __asm volatile(
        "MOVS R3, #3\n"
        "MSR CONTROL, R3\n" // Use PSP & Unpriv thread mode
        "MSR PSP, R0\n"     // Set PSP from argument
        "ISB\n"
        "POP {R4-R11}\n" // Restore callee-saved registers
        "LDR LR, =0xFFFFFFFD\n"
        "BX LR\n");
}

void task_swtch(int nextid)
{
    yield_source = YIELD_MANUAL;
    yield_manual_next = nextid;
    SCB_ICSR |= SCB_ICSR_PENDSVSET; // Set PendSV to trigger

    // This will correctly save the current context

    /*
    current_task_id = nextid;
    current_task = &(tasks[nextid]);

    register tcb_t *next_tcb asm("r0") = current_task;
    task_swtch_impl(next_tcb);
*/
    // Actually will not reach here and will not return
}

/*__attribute__((naked)) void task_swtch_impl(tcb_t *next)
{
    __asm volatile(
        // "BL task_select\n" // R0 = new PSP
        // Save return value (next task pointer) into current_task
        // "LDR R1, =current_task\n"
        // "STR R0, [R1]\n"

        // Load the stack pointer of the new task
        "LDR R2, [R0]\n"        // R2 = new task->sp
        "LDMIA R2!, {R4-R11}\n" // Restore R4-R11 from new stack
        "MSR PSP, R2\n"         // Update PSP to point to new stack position

        "LDR R0, =0xFFFFFFFD\n"
        "MOV LR, R0\n"

        "BX LR\n" // Return from exception, resumes the task
    );
}*/

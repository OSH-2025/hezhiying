# About STM32 context switch (`GPT 4o`)

# Q1

```
Write a STM32 ARM assembly routine that will save all context to a specified C struct and show how to restore them. Ans show how to call the routine from C code.
```

Here's how to write an ARM assembly routine for STM32 that saves the full CPU context (registers and status registers) into a specified C struct and then restores it later.

---

## 🧠 **Assumptions**

-   Target: Cortex-M (STM32F1/F4/etc., based on ARMv7-M architecture)
-   Toolchain: ARM GCC
-   The C struct holds:
    -   General-purpose registers: `R0–R12`
    -   Stack pointer: `SP`
    -   Link register: `LR`
    -   Program counter: `PC`
    -   Program status register: `xPSR`

> **Note:** `PC`, `SP`, and `xPSR` are special registers and require special handling.

---

## 🧱 **1. Define the C Struct**

```c
typedef struct {
    uint32_t r[13];   // R0 - R12
    uint32_t sp;      // Stack Pointer
    uint32_t lr;      // Link Register
    uint32_t pc;      // Program Counter
    uint32_t xpsr;    // Program Status Register
} CPU_Context;
```

---

## ⚙️ **2. Assembly Routine (save/restore)**

File: `context.S`

```assembly
.syntax unified
.cpu cortex-m3
.thumb

.global save_context
.global restore_context

// void save_context(CPU_Context* ctx)
save_context:
    // R0 contains pointer to CPU_Context
    PUSH {R1-R12, LR}

    // Save R0-R12
    STR R0, [R0]            // Save R0
    ADD R1, R0, #4
    STR R1, [R0, #4]        // Save R1
    ADD R1, R0, #8
    STR R2, [R0, #8]
    STR R3, [R0, #12]
    STR R4, [R0, #16]
    STR R5, [R0, #20]
    STR R6, [R0, #24]
    STR R7, [R0, #28]
    STR R8, [R0, #32]
    STR R9, [R0, #36]
    STR R10, [R0, #40]
    STR R11, [R0, #44]
    STR R12, [R0, #48]

    // Save SP
    MRS R1, MSP
    STR R1, [R0, #52]

    // Save LR
    LDR R1, [SP, #0]      // LR was pushed first
    STR R1, [R0, #56]

    // Save PC - just save return address (next instruction)
    LDR R1, [SP, #4]      // PC return address
    STR R1, [R0, #60]

    // Save xPSR
    MRS R1, xPSR
    STR R1, [R0, #64]

    POP {R1-R12, PC}      // Return

// void restore_context(CPU_Context* ctx)
restore_context:
    // Load R0-R12
    LDR R1, [R0]            // R0
    LDR R2, [R0, #4]        // R1
    LDR R3, [R0, #8]
    LDR R4, [R0, #12]
    LDR R5, [R0, #16]
    LDR R6, [R0, #20]
    LDR R7, [R0, #24]
    LDR R8, [R0, #28]
    LDR R9, [R0, #32]
    LDR R10, [R0, #36]
    LDR R11, [R0, #40]
    LDR R12, [R0, #44]

    // Restore SP
    LDR R1, [R0, #52]
    MSR MSP, R1

    // Restore LR
    LDR LR, [R0, #56]

    // Restore xPSR
    LDR R1, [R0, #64]
    MSR xPSR, R1

    // Restore PC (branch to saved address)
    LDR R1, [R0, #60]
    BX R1
```

---

## 🧪 **3. C Usage Example**

```c
#include <stdint.h>
#include "context.h"

extern void save_context(CPU_Context* ctx);
extern void restore_context(CPU_Context* ctx);

CPU_Context context;

void test_context_switch(void) {
    save_context(&context);

    // ... do something else ...

    restore_context(&context);
}
```

---

## ⚠️ **Important Notes**

-   `MSP` is used here assuming thread mode with main stack.
-   This example doesn't handle floating point (`S0–S31`)—add `VSTM`/`VLDM` if needed.
-   You might need to make the routine `__attribute__((naked))` in some compilers if calling from C and handling SP manually.

---

Would you like a version that uses inline assembly or handles interrupts/FPU too?

# Q2

```
Explain __attribute__((naked))
```

Great question! Let's break it down.

---

## 🧩 `__attribute__((naked))` — What is it?

In GCC (and compatible compilers like ARM GCC), `__attribute__((naked))` tells the compiler:

> ❌ “Don’t generate the usual function prologue and epilogue code.”

---

## 🧠 What does that mean?

Normally, when you call a function in C, the compiler **automatically adds** code to:

1. Push registers (like LR, R4-R11) to the stack.
2. Set up the stack frame (adjust SP).
3. Restore everything before returning.

This is helpful for most functions—but not when you're writing **low-level assembly**, like context switches, interrupt handlers, or custom ISRs, because:

### ➕ You want full control over the CPU registers and stack.

---

## ✍️ Example without `naked`

```c
void my_handler(void) {
    asm volatile("NOP");
}
```

Compiler adds something like this:

```assembly
my_handler:
    push {lr}
    ...
    NOP
    ...
    pop {lr}
    bx lr
```

---

## ✍️ Example with `__attribute__((naked))`

```c
__attribute__((naked)) void my_handler(void) {
    asm volatile("NOP");
}
```

Compiler generates:

```assembly
my_handler:
    NOP
```

No prologue, no epilogue—**you must handle everything yourself** (saving/restoring registers, returning).

---

## 🛑 When to Use It

Use `naked` **only** when:

-   You're writing a function entirely in assembly.
-   You want precise control over registers, especially SP, LR, PC.
-   You're doing context switches, interrupt trampolines, or fast ISR stubs.

---

## ⚠️ Gotchas

-   Never mix normal C code with `naked` functions—undefined behavior.
-   Don't return with `return;` — use `BX LR` or `POP {PC}` manually.
-   No local variables or function calls inside a `naked` function.

---

## ✅ Good Use Case

```c
__attribute__((naked)) void PendSV_Handler(void) {
    __asm volatile (
        "MRS R0, PSP         \n"
        "STMDB R0!, {R4-R11} \n"
        "LDR R1, =context    \n"
        "STR R0, [R1]        \n"
        "..." // switch stack pointer to next task
        "..." // load next context
        "BX LR               \n"
    );
}
```

Want help wrapping your context switching code in a `naked` function properly?

# Q3

```
But your code use R1 as intermediate register when restoring PC. This will affect R1 content. Is it?
```

Yes! 💡 You're absolutely right to spot that — and that’s a **critical detail** in context switching or any low-level CPU state restoration.

---

## 🔍 The Issue

In the `restore_context` routine I gave earlier, this line is key:

```assembly
LDR R1, [R0, #60]  // Load saved PC
BX R1              // Branch to saved PC
```

Here, we use **`R1`** as a temporary register to load the program counter (`PC`) and jump to it.

BUT — if you’re restoring **all registers**, and `R1` is supposed to be restored from saved context, then using `R1` as a temp will overwrite what should have gone into it. 😬

---

## 🛠️ Solution Options

### ✅ Option 1: Use a callee-saved register not being restored

If you haven't yet written to `R1`, you could use a different scratch register **after** restoring everything else — say, `R14` (`LR`) or even `R3`, if it's not important at that moment. But usually this is tricky because **all general-purpose registers are supposed to be restored**.

### ✅ Option 2: Restore all at once using `LDMIA`

You can avoid the issue altogether by restoring multiple registers including the PC in a single instruction:

```assembly
LDMIA R0!, {R0-R12}     // Load R0-R12 from context
LDR SP, [R0]            // Load SP
LDR LR, [R0, #4]        // Load LR
LDR R14, [R0, #4]
LDR R1, [R0, #8]        // Temporarily use R1 to hold PC
LDR R2, [R0, #12]       // xPSR
MSR xPSR, R2
BX R1                  // Branch to restored PC
```

Still using `R1` here, but maybe we already restored R1 from memory — **so this needs careful ordering**.

---

### ✅ Option 3: Push PC directly into stack and `POP {PC}`

Another trick is to **load PC into the stack** and then:

```assembly
LDR R1, [R0, #60]    // Load saved PC
PUSH {R1}
POP {PC}             // Return via saved PC
```

This keeps R1 trashed, but at least preserves other registers, and it's clearer you're about to jump. Still not perfect if R1 is being restored.

---

### ✅ Option 4: Restore all with one `LDMFD` if stored right

If the struct is laid out in memory so that all registers including `PC` are in one contiguous block in the right order, you can do:

```assembly
LDMIA R0, {R0-R12, SP, LR, PC}  // Only works on privileged mode
```

But ARM **does not allow PC in LDMIA** unless you're in **ARM state or handler mode**, not always usable in Thumb or Cortex-M.

---

## 🔐 Best Practice

When you're restoring context and need to preserve every bit, including all GPRs:

-   **Restore GPRs first**, including R1
-   Use `POP {PC}` or `BX LR` to jump back
-   Avoid clobbering any register that's being restored

---

## 🧼 Clean version (without clobbering R1)

```assembly
restore_context:
    // Restore R0–R12
    LDMIA R0!, {R0-R12}

    // Restore SP, LR, xPSR, PC
    LDR SP, [R0]        // SP
    LDR LR, [R0, #4]    // LR
    LDR R2, [R0, #8]    // PC
    LDR R3, [R0, #12]   // xPSR
    MSR xPSR, R3
    BX R2               // Branch to saved PC
```

✅ Here we **don’t touch R1**, so it's restored properly too.

---

Let me know if you'd like a reworked version of the full save/restore pair with this fix applied!

# Q4

```
Please explain how the last shown code avoid touching R2, or is it defined by GCC arm as caller saved?
```

Excellent follow-up — you're asking exactly the kind of question that matters when you're doing precise low-level work. Let's get into it:

---

## 💡 Short Answer:

👉 **No, the last code does _not_ avoid touching `R2`**, and that's okay **because `R2` is caller-saved** according to the **ARM Procedure Call Standard (AAPCS)**.

---

## 🧠 What does "caller-saved" mean?

In the ARM calling convention:

| Register | Role                         | Saved by   |
| -------- | ---------------------------- | ---------- |
| `R0–R3`  | Argument/result registers    | **Caller** |
| `R4–R11` | Callee-saved registers       | **Callee** |
| `R12`    | Intra-procedure-call scratch | **Caller** |
| `R13`    | Stack Pointer (SP)           | —          |
| `R14`    | Link Register (LR)           | —          |
| `R15`    | Program Counter (PC)         | —          |

So:

-   If a function (like your context restore routine) **uses R2**, it doesn't need to preserve it before modifying it.
-   It’s the **caller’s responsibility** to save R2 if it needs its value after the call.

---

## ✅ Why this is okay in your context

In a **context switch** or **interrupt return** scenario:

-   You’re restoring the _entire_ CPU state from memory — not just returning to your caller.
-   You're **not returning to C code** at all — you're jumping into a thread/task/ISR with a full context (R0–R12, SP, LR, PC, xPSR).

Thus:

-   It’s **safe to use R2 (or any caller-saved register)** as scratch, because you're replacing its value anyway from the saved context.

---

## 🛡️ Want to be extra cautious?

If you **truly want to avoid clobbering any registers**, then:

-   Don’t use any GPR (`R0–R12`) directly for scratch if you're restoring them all.
-   Instead:
    -   Use **stack** (e.g. push the value, then pop PC).
    -   Or rearrange code to use registers _after_ they've been restored.

---

## ✅ Summary

-   ✅ **R2 is caller-saved**, so it's safe to use in `restore_context`.
-   🧠 In a full context restore, **nothing is preserved**, so clobbering is irrelevant unless you're doing partial saves.
-   🧼 If you're writing truly "transparent" assembly (used from C), be careful which registers you touch.

---

Would you like me to show a totally register-neutral version that touches _no GPRs_ unnecessarily — even if it means going through the stack a bit more?

# Q5

```
Concluding all the above, I want two routines in my STM32 project to allow pseudo-multitasking, which requires task switch. Write the code and explain it.
```

Great! Let's wrap all the above ideas into two clean and functional **ARM Cortex-M** assembly routines to enable **pseudo-multitasking via cooperative context switching** in your **STM32 project**.

---

## 🎯 Objective

We’ll implement two routines:

1. `save_context`: Save CPU registers into a C struct.
2. `restore_context`: Restore CPU registers from a C struct.

We'll also provide:

-   A simple `TaskContext` struct in C.
-   C code that shows how to switch tasks.
-   Detailed comments for learning and clarity.

---

## 🧠 Assumptions

-   Target: STM32 Cortex-M (M3/M4/M7 etc.)
-   Using ARM Thumb mode (standard for Cortex-M).
-   Tasks run in **thread mode**, not handler mode.
-   Context stored manually (not relying on exception stack frame).
-   Pseudo-multitasking = cooperative (tasks yield manually).

---

## 🧱 C Struct Definition

```c
// task.h
#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct {
    uint32_t r4_r11[8];   // Callee-saved registers R4–R11
    uint32_t sp;          // Stack Pointer
    uint32_t lr;          // Link Register (optional)
    uint32_t pc;          // Program Counter
    uint32_t xpsr;        // Program status (needed for Thumb bit)
} TaskContext;

#endif
```

---

## 🔧 Assembly Code

Save this in a `.S` file (e.g. `context_switch.S`) and ensure it's included in your STM32 project.

```assembly
.syntax unified
.cpu cortex-m4
.thumb

.global save_context
.global restore_context

// void save_context(TaskContext* ctx)
.type save_context, %function
save_context:
    // R0 = ctx pointer

    // Save R4–R11
    STMIA R0!, {R4-R11}      // 8 x 4 = 32 bytes

    // Save SP
    MRS R1, PSP              // Get Process Stack Pointer
    STR R1, [R0], #4

    // Save LR
    MOV R2, LR
    STR R2, [R0], #4

    // Save PC manually via literal return address (caller must pass it)
    LDR R2, [SP]             // Get return address from stack (LR stored here)
    STR R2, [R0], #4

    // Save xPSR
    MRS R2, xPSR
    STR R2, [R0]

    BX LR                    // Return to caller
.size save_context, . - save_context

// void restore_context(TaskContext* ctx)
.type restore_context, %function
restore_context:
    // R0 = ctx pointer

    // Restore R4–R11
    LDMIA R0!, {R4-R11}

    // Restore SP
    LDR R1, [R0], #4
    MSR PSP, R1

    // Restore LR
    LDR LR, [R0], #4

    // Restore PC and xPSR
    LDR R2, [R0], #4   // PC
    LDR R3, [R0]       // xPSR

    MSR xPSR, R3
    BX R2              // Jump to restored PC
.size restore_context, . - restore_context
```

---

## ✅ Calling from C

```c
#include "task.h"

extern void save_context(TaskContext* ctx);
extern void restore_context(TaskContext* ctx);

TaskContext task1_ctx;
TaskContext task2_ctx;

void task1(void) {
    while (1) {
        // ... do some work ...
        save_context(&task1_ctx);
        restore_context(&task2_ctx);
    }
}

void task2(void) {
    while (1) {
        // ... do some work ...
        save_context(&task2_ctx);
        restore_context(&task1_ctx);
    }
}
```

---

## 🔄 Task Initialization (e.g., on startup)

Before using `restore_context()` for the first time on a new task, you must **initialize the stack** manually.

Example:

```c
void init_task(TaskContext* ctx, void (*entry)(void), uint32_t* stack_top) {
    ctx->sp = (uint32_t)stack_top;
    ctx->lr = 0xFFFFFFFD;         // Indicates return to thread mode, use PSP
    ctx->pc = (uint32_t)entry;
    ctx->xpsr = 0x01000000;       // Default Thumb bit set

    // Zero out R4–R11
    for (int i = 0; i < 8; ++i)
        ctx->r4_r11[i] = 0;
}
```

Call it like this:

```c
#define STACK_SIZE 256
uint32_t task1_stack[STACK_SIZE];
uint32_t task2_stack[STACK_SIZE];

int main(void) {
    init_task(&task1_ctx, task1, &task1_stack[STACK_SIZE - 1]);
    init_task(&task2_ctx, task2, &task2_stack[STACK_SIZE - 1]);

    // Start first task
    restore_context(&task1_ctx);

    while (1); // Never reached
}
```

---

## 🔐 Why This Works

-   You're manually saving **callee-saved registers** (`R4–R11`) and the **stack pointer**, **program counter**, and **xPSR**.
-   On task switch, the full CPU state is restored—so the new task resumes **as if it was never interrupted**.
-   Cooperative switching means the task itself decides when to yield.

---

## ⚠️ Final Tips

-   For preemptive multitasking, you'd need an interrupt like `SysTick` or `PendSV`.
-   Make sure each task has its **own stack**.
-   Always keep Thumb bit (bit 24) set in `xPSR`.

---

Want to take this further and add preemptive scheduling or integrate with `PendSV_Handler`? Let me know and I’ll build on this.

# Q6

```
Explain thumb bit
```

Great question! The **Thumb bit** is a crucial part of ARM Cortex-M programming, especially when you're doing **manual context switching** or low-level assembly work.

---

## 🧠 What is the Thumb Bit?

The **Thumb bit** is **bit 24** of the **Program Status Register (xPSR)** in ARM Cortex-M processors.

-   It's often called **T-bit**.
-   It indicates whether the processor is executing in **Thumb mode** or **ARM mode**.

---

## 🤔 Why does it matter?

### On Cortex-M:

-   **Only Thumb mode is supported**.
-   All instructions must be **16-bit or 32-bit Thumb instructions**.
-   So, **T-bit must always be set to 1**.

If it's **cleared to 0**, the processor will try to interpret instructions as **ARM instructions**, which **don't exist on Cortex-M**, resulting in a **HardFault**.

---

## 🔍 Where do you find the T-bit?

It's bit 24 of `xPSR`, which gets loaded into the CPU when:

-   Returning from an interrupt.
-   Manually setting context via `MSR xPSR, R3` (like in your context switching code).
-   Using special return instructions like `BX` or `POP {…, PC}`.

You can set it like this:

```c
ctx->xpsr = 0x01000000; // Bit 24 = 1 => Thumb mode
```

---

## 🛠️ Real Example: Why you must set the T-bit

Say you initialize a task’s PC like this:

```c
ctx->pc = (uint32_t)task1;
ctx->xpsr = 0x01000000; // T-bit set
```

If you forget the `xpsr` bit:

```c
ctx->xpsr = 0x00000000; // BAD: T-bit cleared
```

Then when your context restore code does:

```assembly
MSR xPSR, R3
BX R2     ; R2 contains PC
```

💥 The processor switches to ARM mode (which it doesn’t support), causing a **HardFault**.

---

## ✅ TL;DR

| Thumb Bit (T-bit) | Meaning       | Valid on Cortex-M? |
| ----------------- | ------------- | ------------------ |
| `1`               | Thumb mode ✅ | ✅ Required        |
| `0`               | ARM mode ❌   | ❌ Causes crash    |

Always set bit 24 (`0x01000000`) in `xPSR` when initializing context manually.

---

Let me know if you want to see how the T-bit fits into actual binary instruction decoding or want a memory dump visual!

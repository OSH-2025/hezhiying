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
#include "../lightrt.h"

void svcHandler(uint32_t *svc_args);

__attribute__((naked)) void sv_call_handler(void)
{
    __asm volatile(
        "TST lr, #4            \n" // Test bit 2 of LR to know which stack pointer to use
        "ITE EQ                \n"
        "MRSEQ r0, MSP         \n" // r0 = MSP if bit 2 is 0
        "MRSNE r0, PSP         \n" // r0 = PSP if bit 2 is 1
        "B svcHandler          \n" // Branch to C handler
    );
}

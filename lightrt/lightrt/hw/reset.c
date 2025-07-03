#include "../lightrt.h"

#ifdef LRT_USE_RENODE
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/vector.h>

/* load optional platform dependent initialization routines */
// #include "../dispatch/vector_chipset.c"
// #include "../libopencm3/lib/dispatch/vector_chipset.c"
/* load the weak symbols for IRQ_HANDLERS */
// #include "../dispatch/vector_nvic.c"
// #include "../libopencm3/lib/dispatch/vector_nvic.c"

typedef void (*funcp_t)(void);
extern funcp_t __preinit_array_start, __preinit_array_end;
extern funcp_t __init_array_start, __init_array_end;
extern funcp_t __fini_array_start, __fini_array_end;

int main(void);

void reset_handler(void)
{
    /* volatile unsigned *src, *dest; */
    funcp_t *fp;

    /* for (src = &_data_loadaddr, dest = &_data;
         dest < &_edata;
         src++, dest++)
    {
        *dest = *src;
    }

    while (dest < &_ebss)
    {
        *dest++ = 0;
    } */

    /* Ensure 8-byte alignment of stack pointer on interrupts */
    /* Enabled by default on most Cortex-M parts, but not M3 r1 */
    SCB_CCR |= SCB_CCR_STKALIGN;

    /* Constructors. */
    for (fp = &__preinit_array_start; fp < &__preinit_array_end; fp++)
    {
        (*fp)();
    }
    for (fp = &__init_array_start; fp < &__init_array_end; fp++)
    {
        (*fp)();
    }

    /* Call the application's entry point. */
    (void)main();

    /* Destructors. */
    for (fp = &__fini_array_start; fp < &__fini_array_end; fp++)
    {
        (*fp)();
    }
}
#endif

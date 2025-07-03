#include "../lightrt.h"

uint32_t svc_rand_seed(int seed)
{
    rand_seed(&(current_task->tls_area.rand_state), seed);

    return E_SUCCESS;
}

uint32_t svc_rand_next()
{
    return rand_next(&(current_task->tls_area.rand_state));
}
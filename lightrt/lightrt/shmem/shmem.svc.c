#include "../lightrt.h"

uint32_t svc_shmem_size()
{
    // According to the implement of shared memory controller
    // offset +0: memory size (bytes)
    // offset +4: memory state (0 = unlock, 1 = lock by this, 2 = locked by other) Write: 0 = Change to unlock, 1 = Try lock (check by read and found 1)
    // offset +8 and after: the memory itself

    return *maddr(0x60000000);
}

uint32_t svc_shmem_lock()
{
    *maddr(0x60000004) = 1;
    return *maddr(0x60000004) == 1 ? E_SUCCESS : E_OCCUPIED;
}

uint32_t svc_shmem_base()
{
    // This is currently a constant.
    // Change this when changing the shared memory controller.
    return 0x60000008;
}

uint32_t svc_shmem_unlock()
{
    *maddr(0x60000004) = 0;
    return *maddr(0x60000004) == 0 ? E_SUCCESS : E_OCCUPIED;
}
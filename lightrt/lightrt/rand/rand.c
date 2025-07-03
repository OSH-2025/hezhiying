#include "rand.h"

void rand_seed(uint32_t *state, uint32_t s)
{
    if (s == 0)
        s = 2463534242; // fallback if seed is zero
    *state = s;
}

uint32_t rand_next(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

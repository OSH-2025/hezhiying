#include "../lightrt.h"

#ifndef _RAND_H
#define _RAND_H

void rand_seed(uint32_t *state, uint32_t s);
uint32_t rand_next(uint32_t *state);

#endif // RAND32_H

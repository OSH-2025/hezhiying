#include "../lightrt.h"

#ifndef _CROSAA_H
#define _CROSAA_H
#include <stdint.h>

void gpio_led_init(uint32_t port, uint32_t pin);
void gpio_led_set(uint32_t port, uint32_t pin, int on);

#endif // _CROSAA_H
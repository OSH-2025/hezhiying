#include "../lightrt.h"

uint32_t svc_gpio_led_init(uint32_t port, uint32_t pin)
{
    gpio_led_init(port, pin);
    return E_SUCCESS;
}

uint32_t svc_gpio_led_set(uint32_t port, uint32_t pin, int on)
{
    gpio_led_set(port, pin, on);
    return E_SUCCESS;
}
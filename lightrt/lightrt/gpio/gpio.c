#include "gpio.h"

// 初始化 LED（可选，放在 main 或 init 里调用）
void gpio_led_init(uint32_t port, uint32_t pin)
{
    // 这里只处理 GPIOA/B/C...，实际可根据需要扩展
    if (port == GPIOA)
        rcc_periph_clock_enable(RCC_GPIOA);
    else if (port == GPIOB)
        rcc_periph_clock_enable(RCC_GPIOB);
    else if (port == GPIOC)
        rcc_periph_clock_enable(RCC_GPIOC);
    // 可继续扩展其它端口

    gpio_set_mode(port, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, pin);
}

// 控制LED开关
void gpio_led_set(uint32_t port, uint32_t pin, int on)
{
    if (on)
        gpio_set(port, pin);
    else
        gpio_clear(port, pin);
}

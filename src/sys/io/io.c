/*
LightRT : Coursework for 'Hezhiying' team @ OSH 2025, USTC
~~~~~~~

io.c - IO 设备控制实现（蜂鸣器、LED、按键输入）

*/

#include "io.h"
#include "../timer/timer.h"
#include "../task/task.h"

#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)
#define GPIOA_CRL (*(volatile uint32_t *)0x40010800)
#define GPIOA_ODR (*(volatile uint32_t *)0x4001080C)
#define GPIOB_CRL (*(volatile uint32_t *)0x40010C00)
#define GPIOB_IDR (*(volatile uint32_t *)0x40010C08)
#define RCC_APB1ENR (*(volatile uint32_t *)0x4002101C)
#define TIM2_CR1 (*(volatile uint32_t *)0x40000000)
#define TIM2_ARR (*(volatile uint32_t *)0x4000002C)
#define TIM2_CCR1 (*(volatile uint32_t *)0x40000034)
#define TIM2_CCMR1 (*(volatile uint32_t *)0x40000018)
#define TIM2_CCER (*(volatile uint32_t *)0x40000020)
#define TIM2_PSC (*(volatile uint32_t *)0x40000028)

int _io_init(void)
{
    RCC_APB2ENR |= (1 << 2) | (1 << 3);
    RCC_APB1ENR |= (1 << 0);

    // 引脚 5
    GPIOA_CRL &= ~(0xF << 20);
    GPIOA_CRL |= (0x2 << 20);

    // 引脚 0
    GPIOB_CRL &= ~(0xF << 0);
    GPIOB_CRL |= (0x8 << 0);
    GPIOB_ODR |= (1 << 0);

    // 引脚 0
    GPIOA_CRL &= ~(0xF << 0);
    GPIOA_CRL |= (0xB << 0);

    // PWM
    TIM2_PSC = 72 - 1;
    TIM2_ARR = 1000 - 1;
    TIM2_CCMR1 |= (6 << 4);
    TIM2_CCER |= (1 << 0);
    TIM2_CR1 |= (1 << 0);

    return STATUS_SUCCESS;
}

int _buzzer_set(buzzer_freq freq)
{
    if (freq > 20000)
    {
        return STATUS_INVALID_PARAMETER;
    }

    if (freq == 0)
    {
        TIM2_CCR1 = 0;
    }
    else
    {
        // ARR
        uint32_t arr = (1000000 / freq) - 1;
        if (arr < 10)
        {
            return STATUS_INVALID_PARAMETER;
        }
        TIM2_ARR = arr;
        TIM2_CCR1 = arr / 2;
    }

    return STATUS_SUCCESS;
}

int _led_set(int state)
{
    if (state != 0 && state != 1)
    {
        return STATUS_INVALID_PARAMETER;
    }

    if (state)
    {
        GPIOA_ODR |= (1 << 5);
    }
    else
    {
        GPIOA_ODR &= ~(1 << 5);
    }

    return STATUS_SUCCESS;
}

int _key_get(int *state)
{
    if (PTRWRONG(state))
    {
        return STATUS_INVALID_PARAMETER;
    }

    *state = (GPIOB_IDR & (1 << 0)) ? 0 : 1;

    return STATUS_SUCCESS;
}
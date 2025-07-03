#ifndef _LIGHTRT_H
#define _LIGHTRT_H

#ifndef STM32F1
#define STM32F1
#endif

#define LRT_USE_RENODE

#include <stdint.h>

#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>

#include "task/task.h"
#include "uart/uart.h"
#include "syscall/svc.h"
#include "errno.h"
#include "signal/signal.h"
#include "mm/mm.h"
#include "rand/rand.h"
#include "gpio/gpio.h"
#include "dma/dma.h"

// Utility macros
#define maddr(p) ((volatile uint32_t *)(p))

#define mxsram __attribute__((section(".xsram"), used))

#endif
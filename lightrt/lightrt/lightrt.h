#ifndef _LIGHTRT_H
#define _LIGHTRT_H

#ifndef STM32F1
#define STM32F1
#endif

#include <stdint.h>

#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include "task.h"
#include "uart.h"
#include "svc.h"
#include "errno.h"
#include "signal.h"

#endif
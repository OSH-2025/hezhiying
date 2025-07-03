#include "../lightrt.h"

#ifndef _UART_H
#define _UART_H

#include <stdarg.h>

extern int uart_wait_taskid;

void uart_write(const char *str);
void uart_putchar(int ch);
void uart_setup();
void uart_printf(const char *format, ...);

void uart_cancel_wait();

#endif
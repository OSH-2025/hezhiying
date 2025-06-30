#ifndef _UART_H
#define _UART_H

#include "lightrt.h"
#include <stdarg.h>

void uart_write(const char *str);
void uart_putchar(int ch);
void uart_setup();
void uart_printf(const char *format, ...);

#endif
#include "uart.h"

void uart_write(const char *str)
{
    while (*str)
    {
        usart_send_blocking(USART1, *str++);
    }
}

void uart_putchar(int ch)
{
    usart_send_blocking(USART1, ch);
}

void uart_setup()
{
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configure pins
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT, GPIO10);

    // USART config
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_enable(USART1);
}

void printui(unsigned int num, int base)
{
    char digits[] = "0123456789abcdef";
    char buffer[32];
    int i = 0;

    if (num == 0)
    {
        uart_putchar('0');
        return;
    }

    while (num > 0)
    {
        buffer[i++] = digits[num % base];
        num /= base;
    }

    while (i--)
    {
        uart_putchar(buffer[i]);
    }
}

void uart_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'd':
            {
                int d = va_arg(args, int);
                if (d < 0)
                {
                    uart_putchar('-');
                    d = -d;
                }
                printui((unsigned int)d, 10);
                break;
            }
            case 'u':
            {
                unsigned int u = va_arg(args, unsigned int);
                printui(u, 10);
                break;
            }
            case 'x':
            {
                unsigned int x = va_arg(args, unsigned int);
                printui(x, 16);
                break;
            }
            case 'o':
            {
                unsigned int o = va_arg(args, unsigned int);
                printui(o, 8);
                break;
            }
            case 'c':
            {
                char c = (char)va_arg(args, int);
                uart_putchar(c);
                break;
            }
            case 's':
            {
                char *s = va_arg(args, char *);
                while (*s)
                    uart_putchar(*s++);
                break;
            }
            case '%':
            {
                uart_putchar('%');
                break;
            }
            default:
                uart_putchar('%');
                uart_putchar(*format);
                break;
            }
        }
        else
        {
            uart_putchar(*format);
        }
        format++;
    }

    va_end(args);
}

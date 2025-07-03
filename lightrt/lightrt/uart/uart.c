#include "uart.h"

void uart_write(const char *str)
{
    while (*str)
    {
        uart_putchar(*str++);
    }
}

void uart_putchar(int ch)
{
    if (ch == '\n')
    {
        usart_send_blocking(USART1, '\r');
        usart_send_blocking(USART1, '\n');
    }
    else
    {
        usart_send_blocking(USART1, ch);
    }
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

    // Allow RX interrupt
    usart_enable_rx_interrupt(USART1);

    /* Enable USART1 interrupt in NVIC */
    nvic_enable_irq(NVIC_USART1_IRQ);
}

void uart_cancel_wait()
{
    uart_wait_taskid = -1;
}

void usart1_isr(void)
{
    // uart_write("RECV\n");

    /* Check if RX not empty flag is set */
    if (usart_get_flag(USART1, USART_SR_RXNE))
    {
        char ch = usart_recv(USART1);
        // uart_putchar(ch);

        if (uart_wait_taskid != -1 && tasks[uart_wait_taskid].state == TASK_WAIT)
        {
            tcb_t *ptask = &tasks[uart_wait_taskid];

            if (ptask->wait_area.wait_uart_pbuf < ptask->wait_area.wait_uart_ebuf)
            {
                // uart_printf("==== UART pbuf = %x ebuf = %x.\n", ptask->wait_area.wait_uart_pbuf, ptask->wait_area.wait_uart_ebuf);
                *(ptask->wait_area.wait_uart_pbuf) = ch;
                ptask->wait_area.wait_uart_pbuf++;
                // uart_printf("#### UART pbuf = %x ebuf = %x.\n", ptask->wait_area.wait_uart_pbuf, ptask->wait_area.wait_uart_ebuf);

                if (ptask->wait_area.wait_uart_pbuf == ptask->wait_area.wait_uart_ebuf)
                {
                    // uart_printf("UART satisfied.\n");

                    // Read finished.
                    uart_wait_taskid = -1;
                    ptask->state = TASK_NORMAL;
                }
            }
            else
            {
                // Bad bptr -- should be return to normal on last read or 0 size buffer given.
                // This could be used to trigger the task but read nothing -- the reading is done by the thread.
                // Add immediate yield (to that task) if fast response is required.
                uart_wait_taskid = -1;
                ptask->state = TASK_NORMAL;
            }
        }
    }
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

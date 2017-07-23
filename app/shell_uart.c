/*
 * shell_uart.c
 *
 *  Created on: Feb 6, 2017
 *      Author: 31442
 */

#include "shell_uart.h"

/* KL02 bsp interface */
static void uart_console_init(void)
{
	/* UART initialize */
	Debug_Uart_Init(DEBUG_UART_BAUD);
}

static void uart_console_putchar(char ch)
{
	Debug_Uart_PutChar(ch);
}

/* usually we get one byte in the uart interrupt */
static unsigned char uart_console_getchar(void)
{
	unsigned char ch;

	ch = receive_byte;

	return ch;
}

static void uart_console_putstring(char *str)
{
	while(*str != '\0')
	{
		Debug_Uart_PutChar(*str++);
	}
}

console_function_s uart_console =
{
	.init = uart_console_init,
	.putbyte = uart_console_putchar,
	.getbyte = uart_console_getchar,
	.putstring = uart_console_putstring
};
console_function_s *console = &uart_console;


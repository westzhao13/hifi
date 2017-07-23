/*
 * shell_uart.h
 *
 *  Created on: Feb 6, 2017
 *      Author: 31442
 */

#ifndef SHELL_UART_H_
#define SHELL_UART_H_

#include "uart.h"

typedef struct
{
	void (*init)(void);
	void (*putbyte)(char ch);
	unsigned char (*getbyte)(void);
	void (*putstring)(char *str);
}console_function_s;

extern console_function_s *console;



#endif /* SHELL_UART_H_ */

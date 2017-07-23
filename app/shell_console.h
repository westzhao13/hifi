/*
 * shell_console.h
 *
 *  Created on: Feb 6, 2017
 *      Author: 31442
 */

#ifndef SHELL_CONSOLE_H_
#define SHELL_CONSOLE_H_

#include "shell_uart.h"

#define buffer_length (256)
#define arg_length (100)
#define max_arg_num (10)

#define UP    (1)
#define DOWN  (2)
#define LEFT  (3)
#define RIGHT (4)

typedef struct
{
	char buffer[buffer_length];
	unsigned int cnt;
	char argv[max_arg_num][arg_length];
}shell_console_s;

typedef struct
{
	char *str; /* cmd */
	unsigned char arg_num; /* value num include  */
	int (*cmd_function)(char argc, char argv[max_arg_num][arg_length]);
	char *help_str; /* cmd help notice */
}cmd_list;

void shell_init(void);
void shell_put(unsigned char ch);
void shell_puts(char *str);
void shell_gets(void);
void shell_main(void);
void shell_uart_isr(void);

#define for_each_cmd(i, max_cmd) \
	for(i=0; i<max_cmd; i++)

#define get_cmd_num() (sizeof(SHELL_CMD)/sizeof(cmd_list))

#define shell_printf printf


#endif /* SHELL_CONSOLE_H_ */

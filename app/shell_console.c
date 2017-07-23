/*
 * shell_console.c
 *
 *  Created on: Feb 6, 2017
 *      Author: 31442
 */
#include "shell_console.h"
#include <string.h>
#include <stdlib.h>

shell_console_s shell_console;
static char *last_argv[max_arg_num];

#define console_title "\r\nled_debug# "
#define shell_put_title() shell_puts(console_title)

#define SHELL_VERSION "1.0"

static int shell_debug(char argc, char argv[max_arg_num][arg_length])
{
	printf("  -> shell console %s \r\n", SHELL_VERSION);

	return 0;
}

static int shell_strcmp(char *str1, cmd_list *shell_cmd, char argv[max_arg_num][arg_length], unsigned char length);

/* regist your cmd here */
cmd_list SHELL_CMD[] =
{
	{
		"shell_version",
		1,
		shell_debug,
		"shell_version"
	}
};

void shell_init(void)
{
	console->init();
	memset(&shell_console, 0, sizeof(shell_console_s));
	shell_puts("\r\n========= shell debug console v1.0 ========");
	shell_put_title();
}

void shell_put(unsigned char ch)
{
	console->putbyte(ch);
}

void shell_puts(char *str)
{
	console->putstring(str);
}

void shell_gets(void)
{
	unsigned char ch;
	ch = console->getbyte();
	shell_console.buffer[shell_console.cnt++] = ch;
	//printf("strong one receive:%d \r\n", ch);
}

static void shell_check(void)
{
	static unsigned int shell_check_cnt = 0;
    static unsigned char save_arv_cnt = 0, display_cnt = 0, direct_flag = 0;
    char char_tmp[3] = {0};
	char ch;
	unsigned char cmd_num, cmd_sum, direct = 0;
	int cmp_cnt;
	unsigned char mismatch_cnt = 0;

	if(shell_check_cnt != shell_console.cnt)
	{
		ch = shell_console.buffer[shell_check_cnt++];
        
        if(shell_console.cnt >= 3)
        {
            char_tmp[0] = shell_console.buffer[shell_console.cnt-1];
            char_tmp[1] = shell_console.buffer[shell_console.cnt-2];
            char_tmp[2] = shell_console.buffer[shell_console.cnt-3];
            
            if(char_tmp[0]=='A' && char_tmp[1] == 91 && char_tmp[2] == 27)
            {
                //up
                direct = UP;
                if(direct_flag++ == 0)
                {
                    display_cnt = save_arv_cnt;
                    shell_puts("\r");
                    shell_put_title();
                    shell_puts(last_argv[display_cnt]);
                    if(display_cnt)
                        display_cnt--;
                    else
                        display_cnt = 9;
                }
                else if(direct_flag == 3)
                {
                    direct_flag = 0;
                }
            }
            else if(char_tmp[0]=='B' && char_tmp[1] == 91 && char_tmp[2] == 27)
            {
                //down
                direct = DOWN;
            }
            else if(char_tmp[0]=='C' && char_tmp[1] == 91 && char_tmp[2] == 27)
            {
                //right
                direct = RIGHT;
            }
            else if(char_tmp[0]=='D' && char_tmp[1] == 91 && char_tmp[2] == 27)
            {
                //left
                direct = LEFT;
            }
            else 
            {
                memset(char_tmp, 0, 3);
                direct = 0;
            }
        }
        
		/* shell display the number and alphabet */
		if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')
			|| (ch >= '0' && ch<='9') || (ch == 95) || (ch == 32) || (ch == '-'))
		{
            if(!direct)
			    shell_put(ch);
		}
		/* detect the enter button */
		else if(ch == 13)
		{
			//shell_puts("\r\nshell_v1.0#");
			if(shell_console.cnt == 1)
			{
				shell_put_title();
			}
			else
			{
				cmd_sum = get_cmd_num();
				for_each_cmd(cmd_num, cmd_sum) {
					cmp_cnt = shell_strcmp(shell_console.buffer, &SHELL_CMD[cmd_num], shell_console.argv, shell_console.cnt-1);
					if(strlen(SHELL_CMD[cmd_num].str) == cmp_cnt || cmp_cnt == 0)
					{
						/* arguments matched */
						shell_puts("\r\n");
						SHELL_CMD[cmd_num].cmd_function(SHELL_CMD[cmd_num].arg_num, shell_console.argv);
                        strcpy(last_argv[save_arv_cnt++], SHELL_CMD[cmd_num].str);
                        if(save_arv_cnt == max_arg_num)
                        {
                            save_arv_cnt = 0;
                        }
					}
					else
					{
						/* mismatch */
						mismatch_cnt++;
					}
				}

				if(mismatch_cnt == cmd_sum)
				{
					shell_puts("\r\ncan not find command");
					shell_put_title();
					mismatch_cnt = 0;
				}
			}

			memset(&shell_console, 0, sizeof(shell_console_s));
			shell_check_cnt = 0;
		}
		/* tab */
		else if(ch == 9)
		{
			cmd_sum = get_cmd_num();
			for_each_cmd(cmd_num, cmd_sum) {
				cmp_cnt = shell_strcmp(shell_console.buffer, &SHELL_CMD[cmd_num], shell_console.argv, shell_console.cnt-1);
				if(strlen(SHELL_CMD[cmd_num].str) == cmp_cnt)
				{
					/* match */
					memset(&shell_console, 0, sizeof(shell_console_s));
					shell_check_cnt = cmp_cnt;
					shell_console.cnt = strlen(SHELL_CMD[cmd_num].str);
					strcpy(shell_console.buffer, SHELL_CMD[cmd_num].str); //back to where it should be

				}
				else
				{
					/* mismatch */
					if(cmp_cnt > 0)
					{
						//matched sort of words from the head of the cmd string
						memset(&shell_console, 0, sizeof(shell_console_s));
						shell_check_cnt = cmp_cnt;
						shell_console.cnt = strlen(SHELL_CMD[cmd_num].str);
						strcpy(shell_console.buffer, SHELL_CMD[cmd_num].str);
					}
				}
			}
		}
        /* delete */
        else if(ch == 8)
        {
            printf("\b \b");
            if(shell_console.cnt >= 2)
            {
                shell_console.cnt -= 2;
                shell_check_cnt -=2;
            }
        }
        else
        {
           // if(!direct)
                //printf("%d ", ch);
        }
	}
}

void shell_main(void)
{
	shell_check();
}

static int shell_strcmp(char *str1, cmd_list *shell_cmd, char argv[max_arg_num][arg_length], unsigned char length)
{
	unsigned char cmp_cnt = 0, cmd_arg, arg_cnt = 1;
	char *cmd_str;
	unsigned char pCnt;

	cmd_arg = shell_cmd->arg_num;
	cmd_str = shell_cmd->str;
	while(length)
	{
		if((*str1-*cmd_str++)!=0 || !*str1++)
		{
			goto mismatch;
		}
		length--;
		cmp_cnt++;
	}

	return cmp_cnt; /* one argument, if matched it returns length of the arg */

mismatch:
	if((cmp_cnt == strlen(shell_cmd->str)) && (cmd_arg != 1) && (*(str1) == ' '))
	{
		strcpy(argv[0], shell_cmd->str);
		str1++;
		while(arg_cnt < cmd_arg)
		{
			pCnt = 0;
			while((*str1 != ' ') && (*str1 != 0) && (*str1 != 13))
			{
				argv[arg_cnt][pCnt++] = *str1++;
			}
		    arg_cnt++;
			if(*str1 == ' ')
				str1++;
		}
		/* few arguments, if check done it returns 0, and arguments have put into the point argv*/
		return 0;
	}
	return -1; /* mismatch any command */
}

#if 1
/**
 * @brief  
 * we overwrite the uart interrupt function(strong one)
 * @param  None
 * @retval None
 */
void shell_uart_isr(void)
{
    shell_gets();
}
#endif



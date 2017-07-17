/*
 * uart_stdio.c
 *
 *  Created on: 2017Äê7ÔÂ12ÈÕ
 *      Author: westz
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include  <stm32f7xx_hal.h>
#include "uart.h"

/*
 * To implement the STDIO functions you need to create
 * the _read and _write functions and hook them to the
 * USART you are using. This example also has a buffered
 * read function for basic line editing.
 */
int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void get_buffered_line(void);

/*
 * This is a pretty classic ring buffer for characters
 */
#define BUFLEN 127

static uint16_t start_ndx;
static uint16_t end_ndx;
static char buf[BUFLEN + 1];
#define buf_len ((end_ndx - start_ndx) % BUFLEN)
static inline int inc_ndx(int n) { return ((n + 1) % BUFLEN); }
static inline int dec_ndx(int n) { return (((n + BUFLEN) - 1) % BUFLEN); }

/* back up the cursor one space */
static inline void back_up(void)
{
    end_ndx = dec_ndx(end_ndx);
    //usart_send_blocking(USART1, '\010');
    //usart_send_blocking(USART1, ' ');
    //usart_send_blocking(USART1, '\010');

    Debug_Uart_PutChar('\010');
    Debug_Uart_PutChar(' ');
    Debug_Uart_PutChar('\010');
}

/*
 * A buffered line editing function.
 */
void get_buffered_line(void)
{
    char c;

    if (start_ndx != end_ndx)
    {
        return;
    }

    while (1)
    {
        //c = usart_recv_blocking(USART1);
    	c = Debug_Uart_GetChar();

        if (c == '\r')
        {
            buf[end_ndx] = '\n';
            end_ndx = inc_ndx(end_ndx);
            buf[end_ndx] = '\0';
            Debug_Uart_PutChar('\r');
            Debug_Uart_PutChar('\n');
            return;
        }

        /* or DEL erase a character */
        if ((c == '\010') || (c == '\177'))
        {
            if (buf_len == 0)
            {
            	Debug_Uart_PutChar('\a');
            }

            else
            {
                back_up();
            }

            /* erases a word */
        }

        else if (c == 0x17)
        {
            while ((buf_len > 0) &&
                    (!(isspace((int) buf[end_ndx]))))
            {
                back_up();
            }

            /* erases the line */
        }

        else if (c == 0x15)
        {
            while (buf_len > 0)
            {
                back_up();
            }

            /* Non-editing character so insert it */
        }

        else
        {
            if (buf_len == (BUFLEN - 1))
            {
            	Debug_Uart_PutChar('\a');
            }

            else
            {
                buf[end_ndx] = c;
                end_ndx = inc_ndx(end_ndx);
                Debug_Uart_PutChar(c);
            }
        }
    }
}

/*
 * Called by libc stdio fwrite functions
 */
int _write(int fd, char *ptr, int len)
{
    int i = 0;

    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2)
    {
        return -1;
    }

    while (*ptr && (i < len))
    {
    	Debug_Uart_PutString(ptr, len);

        if (*ptr == '\n')
        {
        	Debug_Uart_PutChar('\r');
        }

        i++;
        ptr++;
    }

    return i;
}

/*
 * Called by the libc stdio fread fucntions
 *
 * Implements a buffered read with line editing.
 */
int _read(int fd, char *ptr, int len)
{
    int my_len;

    if (fd > 2)
    {
        return -1;
    }

    get_buffered_line();
    my_len = 0;

    while ((buf_len > 0) && (len > 0))
    {
        *ptr++ = buf[start_ndx];
        start_ndx = inc_ndx(start_ndx);
        my_len++;
        len--;
    }

    return my_len; /* return the length we got */
}

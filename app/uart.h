/*
 * uart.h
 *
 *  Created on: 2017Äê7ÔÂ13ÈÕ
 *      Author: westz
 */

#ifndef APP_UART_H_
#define APP_UART_H_

#include "main.h"

extern UART_HandleTypeDef  DebugUartHandle;
extern uint8_t receive_byte;

/* USER CODE BEGIN Private defines */
#define DEBUG_UART                           USART3
#define DEBUG_UART_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define DEBUG_UART_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define DEBUG_UART_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define DEBUG_UART_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define DEBUG_UART_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define DEBUG_UART_TX_PIN                    GPIO_PIN_8
#define DEBUG_UART_TX_GPIO_PORT              GPIOD
#define DEBUG_UART_TX_AF                     GPIO_AF7_USART3
#define DEBUG_UART_RX_PIN                    GPIO_PIN_9
#define DEBUG_UART_RX_GPIO_PORT              GPIOD
#define DEBUG_UART_RX_AF                     GPIO_AF7_USART3

#define DEBUG_UART_BAUD        (115200)
#define DEBUG_UART_RECV_LEN  (1)
#define DEBUG_UART_TIMEOUT   (100)

void Debug_Uart_Init(uint32_t baud);
void Debug_Uart_PutChar(uint8_t c);
void Debug_Uart_PutString(uint8_t* str, uint8_t len);
uint8_t Debug_Uart_GetChar(void);

#endif /* APP_UART_H_ */

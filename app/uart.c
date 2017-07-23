/*
 * uart.c
 *
 *  Created on: 2017年7月13日
 *      Author: westz
 */
#include "uart.h"

UART_HandleTypeDef  DebugUartHandle;
uint8_t receive_byte;

void Debug_Uart_Init(uint32_t baud)
{
	//UART_HandleTypeDef UartHandle;

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART configured as follows:
	  - Word Length = 8 Bits (7 data bit + 1 parity bit) :
	                  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
	  - Stop Bit    = One Stop bit
	  - Parity      = ODD parity
	  - BaudRate    = 9600 baud
	  - Hardware flow control disabled (RTS and CTS signals) */
	HAL_StatusTypeDef ret;

	DebugUartHandle.Instance        = DEBUG_UART;

	DebugUartHandle.Init.BaudRate   = baud;
	DebugUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	DebugUartHandle.Init.StopBits   = UART_STOPBITS_1;
	DebugUartHandle.Init.Parity     = UART_PARITY_NONE;
	DebugUartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	DebugUartHandle.Init.Mode       = UART_MODE_TX_RX;
	DebugUartHandle.Init.OverSampling = UART_OVERSAMPLING_8;

	ret = HAL_UART_Init(&DebugUartHandle);

	HAL_UART_Receive_IT(&DebugUartHandle, &receive_byte, DEBUG_UART_RECV_LEN);
}


void Debug_Uart_PutChar(uint8_t c)
{
	HAL_UART_Transmit(&DebugUartHandle, &c, 1, DEBUG_UART_TIMEOUT);
}


void Debug_Uart_PutString(uint8_t* str, uint8_t len)
{
	HAL_UART_Transmit(&DebugUartHandle, str, len, DEBUG_UART_TIMEOUT);
}


uint8_t Debug_Uart_GetChar(void)
{
	uint8_t c;

	c = HAL_UART_Receive(&DebugUartHandle, &c, 1, DEBUG_UART_TIMEOUT);
	
    return c;
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  //RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  DEBUG_UART_TX_GPIO_CLK_ENABLE();
  DEBUG_UART_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
  //RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  //RCC_PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
  //HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  DEBUG_UART_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = DEBUG_UART_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_UART_TX_AF;

  HAL_GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = DEBUG_UART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_UART_RX_AF;

  HAL_GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStruct);
  
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  HAL_NVIC_SetPriority(USART3_IRQn,3,3);
  
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  DEBUG_UART_FORCE_RESET();
  DEBUG_UART_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(DEBUG_UART_RX_GPIO_PORT, DEBUG_UART_RX_PIN);

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
		/* add your dealing code here */
		shell_uart_isr();
	}
}


void USART3_IRQHandler(void)
{
	uint32_t timeout=0;
	uint32_t maxDelay=0x1FFFF;

	CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();
	OSIntEnter();                /* Tell OS that we are starting an ISR            */
	CPU_CRITICAL_EXIT();

	//----- HANDLER THE ISR HERE ------
	/* handler */
	HAL_UART_IRQHandler(&DebugUartHandle);
	
	timeout=0;
	/* get ready */
	while (HAL_UART_GetState(&DebugUartHandle)!=HAL_UART_STATE_READY)
	{
		  timeout++;
		  if(timeout>maxDelay) break;
	}

	timeout=0;
	/* restart uart again */
	while(HAL_UART_Receive_IT(&DebugUartHandle, &receive_byte, DEBUG_UART_RECV_LEN)!=HAL_OK)
	{
		  timeout++;
		  if(timeout>maxDelay) break;
	}
	//----- HANDLER THE ISR HERE ------

	OSIntExit();                 /* Tell OS that we are leaving the ISR            */
}


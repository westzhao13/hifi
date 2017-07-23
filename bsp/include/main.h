/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include  <stdint.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <math.h>

#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>

#include  <app_cfg.h>
#include  <bsp.h>
#include  <bsp_led.h>
#include  <bsp_clock.h>

#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

#include "shell_console.h"

#include "uart.h"

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB


typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;
typedef unsigned long long u64;

typedef unsigned char      UInt8;
typedef unsigned short     UInt16;
typedef unsigned long      UInt32;
typedef unsigned long long UInt64;
typedef char	           Int8;
typedef short	           Int16;
typedef long	           Int32;
typedef long long          Int64;

typedef unsigned char      byte;
typedef unsigned char	   BYTE;

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#define _OSA_TRACE_

/* 打印字体显示的颜色 */
#define NONE          "\033[m"   
#define RED           "\033[0;32;31m"   
#define GREEN         "\033[0;32;32m"   
#define BLUE          "\033[0;32;34m"   
#define DARY_GRAY     "\033[1;30m"   
#define CYAN          "\033[0;36m"   
#define PURPLE        "\033[0;35m"     
#define BROWN         "\033[0;33m"   
#define YELLOW        "\033[1;33m"   
#define WHITE         "\033[1;37m"   


/* 模块的名字，每个模块不相同，根据实际修改。*/
#define OSA_MODULE_NAME     "[hifi] "


#ifdef __KERNEL__    /* Linux kernel */
    #include <linux/kernel.h>
    #define OSA_print  printk
#elif ___DSPBIOS___  /* TI SYSBIOS */
    #define OSA_print  Vps_printf /* print info to share memory */
#else                /* Generic Application */
    #include <stdio.h>
    #define OSA_print  printf
#endif


/* _OSA_TRACE_宏是调试开关，可通过Makeife传入，传入方法是-D_OSA_TRACE_。*/
#ifdef _OSA_TRACE_
    /* 断言，输出字体为红色，打印出函数名和行号。程序进入无限循环。*/ 
	#define OSA_assert(x)  \
	    if((x) == 0) { \
		while(1){ \
		OSA_print(RED OSA_MODULE_NAME "ASSERT (%s|%d)\r\n", \
	    	                __func__, __LINE__); \
	    	}  \
	    } 
		
	#define OSA_assertSuccess(ret)	\
	    OSA_assert((ret) == OSA_SOK)

    /* 调试打印，输出字体不带颜色。*/
	#define OSA_DEBUG(fmt, args ...) \
		OSA_print(OSA_MODULE_NAME fmt, ## args)
         
#else
	#define OSA_assert(x)
	#define OSA_assertSuccess(ret)
	#define OSA_DEBUG(fmt, args ... )    
#endif


/* 通用打印，输出字体不带颜色。*/
#define OSA_printf(fmt, args ...) \
	do \
	{ \
		OSA_print(OSA_MODULE_NAME "(%s|%d): " fmt, __func__, __LINE__, ## args);\
	} \
	while(0)

/* 出错打印，输出字体为红色，打印出函数名和行号。表明程序不能运行下去。*/
#define OSA_ERROR(fmt, args ...) \
	do \
	{ \
		OSA_print(RED OSA_MODULE_NAME "ERROR (%s|%d): " fmt NONE, \
		                    __func__, __LINE__, ## args); \
	} \
	while(0)


/* 
* 警告打印，输出字体为黄色，打印出函数名和行号。表明程序仍可运行下去，
* 但须警示。
*/
#define OSA_WARN(fmt, args ...) \
	do \
	{ \
		OSA_print(YELLOW OSA_MODULE_NAME "WARN (%s|%d): " fmt NONE, \
		                    __func__, __LINE__, ## args); \
	} \
	while(0)

/* 通告打印，输出字体为绿色。*/
#define OSA_INFO(fmt, args ...) \
	do \
	{ \
		OSA_print(GREEN OSA_MODULE_NAME "INFO (%s|%d): " fmt NONE, \
		                    __func__, __LINE__, ## args); \
	} \
	while(0)



/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

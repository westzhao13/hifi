/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                          STM32746G-EVAL2
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "main.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_TASK_EQ_0_ITERATION_NBR              16u
#define  APP_TASK_EQ_1_ITERATION_NBR              18u


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/* --------------- APPLICATION GLOBALS ---------------- */
static  OS_STK       AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       ShellTaskStartStk[APP_CFG_TASK_START_STK_SIZE];


/* --------------- SEMAPHORE TASK TEST ---------------- */
static  OS_STK       AppTaskEvent0Stk[APP_CFG_TASK_OBJ_STK_SIZE];
static  OS_STK       AppTaskEvent1Stk[APP_CFG_TASK_OBJ_STK_SIZE];

/* ------------ FLOATING POINT TEST TASK -------------- */
static  OS_STK       AppTaskEq0FpStk[APP_CFG_TASK_EQ_STK_SIZE];
static  OS_STK       AppTaskEq1FpStk[APP_CFG_TASK_EQ_STK_SIZE];

#if (OS_SEM_EN > 0u)
static  OS_EVENT    *AppTaskObjSem;
#endif

#if (OS_MUTEX_EN > 0u)
static  OS_EVENT    *AppTaskObjMutex;
#endif

#if (OS_Q_EN > 0u)
static  OS_EVENT    *AppTaskObjQ;
#endif

#if (OS_FLAG_EN > 0u)
static  OS_FLAG_GRP *AppTaskObjFlag;
#endif

/* Private function prototypes -----------------------------------------------*/

//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskCreate  (void);
static  void  AppEventCreate (void);

static  void  AppTaskStart   (void  *p_arg);

static  void  AppTaskEq0Fp   (void  *p_arg);                   /* Floating Point Equation 0 task.                      */
static  void  AppTaskEq1Fp   (void  *p_arg);                   /* Floating Point Equation 1 task.                      */
static  void  AppTaskEvent0  (void  *p_arg);
static  void  AppTaskEvent1  (void  *p_arg);

static  void  SystemClock_Config(void);
static  void  MX_GPIO_Init(void);
static void CPU_CACHE_Enable(void);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : 1) STM32CubeFx HAL library initialization:
*                      a) Configures the Flash prefetch, intruction and data caches.
*                      b) Configures the Systick to generate an interrupt. However, the function ,
*                         HAL_InitTick(), that initializes the Systick has been overwritten since Micrium's
*                         RTOS has its own Systick initialization and it is recommended to initialize the
*                         Systick after multitasking has started.
*
*********************************************************************************************************
*/

int main(void)
{
	
#if (OS_TASK_NAME_EN > 0)
    CPU_INT08U  err;
#endif

	CPU_CACHE_Enable();
	HAL_Init();                                                 /* See Note 1.                                          */
	SystemClock_Config();
	
	//BSP_SystemClkCfg();
    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    Math_Init();                                                /* Initialize Mathematical Module                       */
    CPU_IntDis();                                               /* Disable all Interrupts.                              */
    OSInit();                                                   /* Init uC/OS-II.                                       */

	MX_GPIO_Init();
	shell_init();

	OSA_INFO(" -> ------------------ system boot ---------------\n");
	OSA_INFO(" -> test \n");

	/*              Create the start task           */
	OSTaskCreateExt( AppTaskStart,
                     0,
                    &AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                     APP_CFG_TASK_START_PRIO,
                     APP_CFG_TASK_START_PRIO,
                    &AppTaskStartStk[0],
                     APP_CFG_TASK_START_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	
#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_START_PRIO,
                  (INT8U *)"Start Task",
                           &err);
#endif
	
	/* Start multitasking (i.e. give control to uC/OS-II).  */
    OSStart();
	
	/* Should Never Get Here.                               */
    while (DEF_ON)
	{
    	OSA_ERROR(" -> program error occured!\n");
    }
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
   (void)p_arg;

#if 1
	/* Initialize BSP functions 							*/
    BSP_Init();
	
	/* Initialize the uC/CPU services						*/
    CPU_Init();
	
#if (OS_TASK_STAT_EN > 0)
	/* Determine CPU capacity								*/
    OSStatInit();
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

#endif
	
	/*               Create Applicaiton kernel objects                    */
    AppEventCreate();

    /*                Create Application tasks                             */
    AppTaskCreate();

	/* Task body, always written as an infinite loop.       */
    while (DEF_TRUE) {
        //BSP_LED_Toggle(0u);
		  HAL_GPIO_WritePin(GPIOB, LD2_Pin, 1);
		  HAL_GPIO_WritePin(GPIOB, LD3_Pin, 0);
		  OSTimeDlyHMSM(0u, 0u, 0u, 200u);
		  //HAL_Delay(1000);
		  HAL_GPIO_WritePin(GPIOB, LD2_Pin, 0);
		  HAL_GPIO_WritePin(GPIOB, LD3_Pin, 1);
          OSTimeDlyHMSM(0u, 0u, 0u, 200u);
          //HAL_Delay(1000);
    }
}

static  void  ShellTastStart (void *p_arg)
{
	/* Task body, always written as an infinite loop.       */
    while (DEF_TRUE) 
	{
		shell_main();
    }
}


/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
#if (OS_TASK_NAME_EN > 0)
    CPU_INT08U  err;
#endif

    /* ----------- CREATE KERNEL EVENT TEST TASK ---------- */
    OSTaskCreateExt( AppTaskEvent0,
                     0,
                    &AppTaskEvent0Stk[APP_CFG_TASK_OBJ_STK_SIZE - 1],
                     APP_CFG_TASK_EVENT0_PRIO,
                     APP_CFG_TASK_EVENT0_PRIO,
                    &AppTaskEvent0Stk[0],
                     APP_CFG_TASK_OBJ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EVENT0_PRIO,
                  (INT8U *)"Kernel Events Task 0",
                           &err);
#endif

    OSTaskCreateExt( AppTaskEvent1,
                     0,
                    &AppTaskEvent1Stk[APP_CFG_TASK_OBJ_STK_SIZE - 1],
                     APP_CFG_TASK_EVENT1_PRIO,
                     APP_CFG_TASK_EVENT1_PRIO,
                    &AppTaskEvent1Stk[0],
                     APP_CFG_TASK_OBJ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EVENT1_PRIO,
                  (INT8U *)"Kernel Events Task 1",
                           &err);
#endif

    /* ------------- CREATE FLOATING POINT TASK ----------- */
    OSTaskCreateExt( AppTaskEq0Fp,
                     0,
                    &AppTaskEq0FpStk[APP_CFG_TASK_EQ_STK_SIZE - 1],
                     APP_CFG_TASK_EQ0_PRIO,
                     APP_CFG_TASK_EQ0_PRIO,
                    &AppTaskEq0FpStk[0],
                     APP_CFG_TASK_EQ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EQ0_PRIO,
                  (INT8U *)"FP  Equation 0",
                           &err);
#endif

    OSTaskCreateExt( AppTaskEq1Fp,
                     0,
                    &AppTaskEq1FpStk[APP_CFG_TASK_EQ_STK_SIZE - 1],
                     APP_CFG_TASK_EQ1_PRIO,
                     APP_CFG_TASK_EQ1_PRIO,
                    &AppTaskEq1FpStk[0],
                     APP_CFG_TASK_EQ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EQ1_PRIO,
                  (INT8U *)"FP  Equation 1",
                           &err);
#endif

#if (OS_TASK_NAME_EN > 0)

	/*              Create the shell task           */
	OSTaskCreate(ShellTastStart,
                 0,
                 &ShellTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                 SHELL_TAST_PRIO);

#endif

}


/*
*********************************************************************************************************
*                                           AppEventCreate()
*
* Description : Create application kernel event tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppEventCreate (void)
{
    CPU_INT08U  os_err;

#if (OS_SEM_EN > 0u)
    AppTaskObjSem = OSSemCreate(0);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet(         AppTaskObjSem,
                   (INT8U *)"Trace Lock",
                            &os_err);
#endif
#endif

#if (OS_MUTEX_EN > 0u)
    AppTaskObjMutex = OSMutexCreate( APP_CFG_MUTEX_PRIO,
                                     &os_err);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet(         AppTaskObjMutex,
                   (INT8U *)"Mutex Test",
                            &os_err);
#endif
#endif

#if (OS_Q_EN > 0u)
    AppTaskObjQ = OSQCreate((void *)&AppTaskObjQ,
                                      1);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet(          AppTaskObjQ,
                   (INT8U *)"Queue Test",
                            &os_err);
#endif
#endif

#if (OS_FLAG_EN > 0u)
    AppTaskObjFlag = OSFlagCreate( DEF_BIT_NONE,
                                   &os_err);

#if (OS_EVENT_NAME_EN > 0)
    OSFlagNameSet(         AppTaskObjFlag,
                  (INT8U *)"Flag Test",
                           &os_err);
#endif
#endif
}


/*
*********************************************************************************************************
*                                          AppTaskEvent0()
*
* Description : Test uC/OS-II Events.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskEvent0' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskEvent0 (void  *p_arg)
{
    CPU_INT08U  os_err;

    (void)p_arg;


    while (DEF_TRUE) {
#if (OS_SEM_EN > 0u)
        os_err = OSSemPost(AppTaskObjSem);
#endif

#if (OS_MUTEX_EN > 0u)
        OSMutexPend( AppTaskObjMutex,
                     0,
                    &os_err);

        OSTimeDlyHMSM(0u, 0u, 0u, 100u);

        os_err = OSMutexPost(AppTaskObjMutex);
#endif

#if (OS_Q_EN > 0u)
        os_err = OSQPost(        AppTaskObjQ,
                         (void *)1u);
#endif

#if (OS_FLAG_EN > 0u)
        OSFlagPost( AppTaskObjFlag,
                    DEF_BIT_00,
                    OS_FLAG_SET,
                   &os_err);
#endif
        OSTimeDlyHMSM(0u, 0u, 0u, 10u);
        APP_TRACE_INFO(("Event test task 0 running ....\n"));
    }
}


/*
*********************************************************************************************************
*                                          AppTaskEvent1()
*
* Description : Test uC/OS-II events.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskEvent1' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskEvent1 (void  *p_arg)
{
    CPU_INT08U  os_err;


    (void)p_arg;

    while (DEF_TRUE) {

#if (OS_SEM_EN > 0u)
        OSSemPend( AppTaskObjSem,
                   0,
                  &os_err);
#endif

#if (OS_MUTEX_EN > 0u)
        OSMutexPend( AppTaskObjMutex,
                     0,
                    &os_err);

        os_err = OSMutexPost(AppTaskObjMutex);
#endif

#if (OS_Q_EN > 0u)
        OSQPend( AppTaskObjQ,
                 0,
                &os_err);
#endif

#if (OS_FLAG_EN > 0u)
        OSFlagPend( AppTaskObjFlag,
                    DEF_BIT_00,
                    OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME,
                    0,
                   &os_err);
#endif
        OSTimeDlyHMSM(0u, 0u, 0u, 10u);
        APP_TRACE_INFO(("Event test task 1 running ....\n"));
    }
}


/*
*********************************************************************************************************
*                                             AppTaskEq0Fp()
*
* Description : This task finds the root of the following equation.
*               f(x) =  e^-x(3.2 sin(x) - 0.5 cos(x)) using the bisection mehtod
*
* Argument(s) : p_arg   is the argument passed to 'AppTaskEq0Fp' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : 1) Atollic TrueSTUDIO needs special settings to output floating point format; however,
*                  IAR and KeilMDK should work properly.
*********************************************************************************************************
*/

void  AppTaskEq0Fp (void  *p_arg)
{
    CPU_FP32    a;
    CPU_FP32    b;
    CPU_FP32    c;
    CPU_FP32    eps;
    CPU_FP32    f_a;
    CPU_FP32    f_c;
    CPU_FP32    delta;
    CPU_INT08U  iteration;
    RAND_NBR    wait_cycles;


    while (DEF_TRUE) {
        eps       = 0.00001;
        a         = 3.0;
        b         = 4.0;
        delta     = a - b;
        iteration = 0u;
        if (delta < 0) {
            delta = delta * -1.0;
        }

        while (((2.00 * eps) < delta) ||
               (iteration    > 20u  )) {
            c   = (a + b) / 2.00;
            f_a = (exp((-1.0) * a) * (3.2 * sin(a) - 0.5 * cos(a)));
            f_c = (exp((-1.0) * c) * (3.2 * sin(c) - 0.5 * cos(c)));

            if (((f_a > 0.0) && (f_c < 0.0)) ||
                ((f_a < 0.0) && (f_c > 0.0))) {
                b = c;
            } else if (((f_a > 0.0) && (f_c > 0.0)) ||
                       ((f_a < 0.0) && (f_c < 0.0))) {
                a = c;
            } else {
                break;
            }

            delta = a - b;
            if (delta < 0) {
               delta = delta * -1.0;
            }
            iteration++;

            wait_cycles = Math_Rand();
            wait_cycles = wait_cycles % 1000;

            while (wait_cycles > 0u) {
                wait_cycles--;
            }

            if (iteration > APP_TASK_EQ_0_ITERATION_NBR) {
                APP_TRACE_INFO(("AppTaskEq0Fp() max # iteration reached\n"));
                break;
            }
        }

        APP_TRACE_INFO(("Eq0 Task Running ....\n"));

        if (iteration == APP_TASK_EQ_0_ITERATION_NBR) {
#if 0                                                           /* See Note 1.                                         */
            APP_TRACE_INFO(("Root = %f; f(c) = %f; #iterations : %d\n", c, f_c, iteration));
#else
            APP_TRACE_INFO(("Eq0 task; #iterations : %d\n", iteration));
#endif
        }

        OSTimeDlyHMSM(0u, 0u, 0u, 10u);
    }
}



/*
*********************************************************************************************************
*                                             AppTaskEq1Fp()
*
* Description : This task finds the root of the following equation.
*               f(x) = x^2 -3 using the bisection mehtod
*
* Argument(s) : p_arg   is the argument passed to 'AppTaskEq()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : 1) Atollic TrueSTUDIO needs special settings to properly output floating point format;
*                  however, IAR and KeilMDK should work properly.
*********************************************************************************************************
*/

void  AppTaskEq1Fp (void  *p_arg)
{
    CPU_FP32    a;
    CPU_FP32    b;
    CPU_FP32    c;
    CPU_FP32    eps;
    CPU_FP32    f_a;
    CPU_FP32    f_c;
    CPU_FP32    delta;
    CPU_INT08U  iteration;
    RAND_NBR    wait_cycles;


    while (DEF_TRUE) {
        eps       = 0.00001;
        a         = 1.0;
        b         = 4.0;
        delta     = a - b;
        iteration = 0u;

        if (delta < 0) {
            delta = delta * -1.0;
        }

        while ((2.00 * eps) < delta) {
            c   = (a + b) / 2.0;
            f_a = a * a - 3.0;
            f_c = c * c - 3.0;

            if (((f_a > 0.0) && (f_c < 0.0)) ||
                ((f_a < 0.0) && (f_c > 0.0))) {
                b = c;
            } else if (((f_a > 0.0) && (f_c > 0.0)) ||
                       ((f_a < 0.0) && (f_c < 0.0))) {
                a = c;
            } else {
                break;
            }

            delta = a - b;
            if (delta < 0) {
               delta = delta * -1.0;
            }
            iteration++;

            wait_cycles = Math_Rand();
            wait_cycles = wait_cycles % 1000;

            while (wait_cycles > 0u) {
                wait_cycles--;
            }

            if (iteration > APP_TASK_EQ_1_ITERATION_NBR) {
                APP_TRACE_INFO(("AppTaskEq1Fp() max # iteration reached\n"));
                break;
            }
        }

        APP_TRACE_INFO(("Eq1 Task Running ....\n"));

        if (iteration == APP_TASK_EQ_1_ITERATION_NBR) {
#if 0                                                           /* See Note 1.                                         */
            APP_TRACE_INFO(("Root = %f; f(c) = %f; #iterations : %d\n", c, f_c, iteration));
#else
            APP_TRACE_INFO(("Eq1 task; #iterations : %d\n", iteration));
#endif
        }

        OSTimeDlyHMSM(0u, 0u, 0u, 10u);
    }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) {};
  }
  
  /* Activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    while(1) {};
  }
  
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    while(1) {};
  }
}
/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}



static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#if 0
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
#endif

#if 0
//lib function "printf" available
#pragma import(__use_no_semihosting)
struct __FILE
{
			 int handle;

};

FILE __stdout;
void _sys_exit(int x)
{
			 x = x;
}

int fputc(int ch, FILE *f)
{
	#if 1
	 while((USART3->ISR&0X40) == 0);
	 USART3->TDR = (uint8_t) ch;
	 return ch;
	#endif
}
#endif


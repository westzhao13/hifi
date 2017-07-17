

*********************************************************************************************************
*                                         ISR HANDLER SETUP
*********************************************************************************************************


(1) 'startup_stm32f746xx.s' defines all the external interrupts as a WEAK reference. So we need to 
     re-defined the handler we would like to use following the template shown below:

     void MyISRHandler (void)
     {
          CPU_SR_ALLOC();               /* Allocate storage for CPU status register      */


          CPU_CRITICAL_ENTER();
          OSIntEnter();                /* Tell OS that we are starting an ISR            */
          CPU_CRITICAL_EXIT();

          ----- HANDLER THE ISR HERE ------
          ----- HANDLER THE ISR HERE ------

          OSIntExit();                 /* Tell OS that we are leaving the ISR            */
     }


(2) For example, if we need to use USART1_IRQHandler defined in 'startup_stm32f746xx.s' as WEAK reference, 
    then we need to re-define the handler in any C file in our project as follow:

     void USART1_IRQHandler (void)
     {
          CPU_SR_ALLOC();


          CPU_CRITICAL_ENTER();
          OSIntEnter();                /* Tell OS that we are starting an ISR            */
          CPU_CRITICAL_EXIT();

          ----- HANDLER THE ISR HERE ------
          ----- HANDLER THE ISR HERE ------

          OSIntExit();                 /* Tell OS that we are leaving the ISR            */
     }


(3) Another example of re-defining an ISR handler


     void ETH_IRQHandler (void)
     {
          CPU_SR_ALLOC();


          CPU_CRITICAL_ENTER();
          OSIntEnter();                /* Tell OS that we are starting an ISR            */
          CPU_CRITICAL_EXIT();

          ----- HANDLER THE ISR HERE ------
          ----- HANDLER THE ISR HERE ------

          OSIntExit();                 /* Tell OS that we are leaving the ISR            */
     }
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>



RCC_ClocksTypeDef RCC_Clocks;

void initGpio()
{
	GPIO_InitTypeDef gpioInitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
  // ********************* PA5	 ***********************
  gpioInitStruct.GPIO_Pin = GPIO_Pin_5;
  gpioInitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
  gpioInitStruct.GPIO_OType = GPIO_OType_PP;
  gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &gpioInitStruct);
	// ****************************************************
	
	// ******************** PC1 ***************************
	gpioInitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &gpioInitStruct);
	// ****************************************************
	
	// ******************** PC0 ***************************
	gpioInitStruct.GPIO_Pin = GPIO_Pin_0;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
	gpioInitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpioInitStruct);
	// ****************************************************
	
	
	// ************************************* silnik **********************************
	
	gpioInitStruct.GPIO_Pin = GPIO_Pin_6;
	gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &gpioInitStruct);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &gpioInitStruct);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOC, &gpioInitStruct);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &gpioInitStruct);
	
	// *******************************************************************************
}	

void initTimers()
{
	NVIC_InitTypeDef nvicStruct;
	TIM_TimeBaseInitTypeDef timerInitStructure;
	
	// ********************** TIM2 *************************
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timerInitStructure.TIM_Prescaler = 20;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 2500000;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	// ******************************************************
	
	/*// *********************** IRQ Timer *************************
	nvicStruct.NVIC_IRQChannel = TIM2_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStruct.NVIC_IRQChannelSubPriority = 1;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStruct);
	// ***************************************************** */
}

void initInterrupts()
{
	/*EXTI_InitTypeDef extiInitStruct;
	NVIC_InitTypeDef nvicStruct;
	// ************************ EXTI0 ******************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);
	nvicStruct.NVIC_IRQChannel = EXTI0_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 0;
  nvicStruct.NVIC_IRQChannelSubPriority = 0;
  nvicStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStruct);
	
	extiInitStruct.EXTI_Line = EXTI_Line0;
	extiInitStruct.EXTI_LineCmd = ENABLE;
	extiInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&extiInitStruct);
	// *************************************************************/
}

void initUART()
{
	GPIO_InitTypeDef     GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	// Initialize pins as alternating function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	 
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
}

void init()
{
	RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	initGpio();
	initTimers();
	initInterrupts();
	initUART();
}

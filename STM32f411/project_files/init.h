#ifndef __MAIN_INIT_H
#define __MAIN_INIT_H 

#include "main.h"
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>


void initGpio(void);
void initTimers(void);
void initInterrupts(void);
void init(void);

const uint16_t TIM2_RESOLUTION_NS = 400;


#else

extern uint16_t TIM2_RESOLUTION_NS;

#endif  

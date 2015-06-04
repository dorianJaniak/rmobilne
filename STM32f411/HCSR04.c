#include "main.h"
#include "HCSR04.h"

extern uint16_t TIM2_RESOLUTION_NS;

double measureDistance()
{
	float distance;
	uint32_t start, stop;
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	TIM_SetCounter(TIM2, 0);
	start = TIM_GetCounter(TIM2);
	while((TIM_GetCounter(TIM2) - start) < 25); // trigger 
	stop = TIM_GetCounter(TIM2);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) == Bit_RESET); //wait for response
	TIM_SetCounter(TIM2, 0);
	start = TIM_GetCounter(TIM2);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) == Bit_SET);
	stop = TIM_GetCounter(TIM2);
	
	distance = (stop - start) * TIM2_RESOLUTION_NS / 1000.0f / 58.0f;
	return distance;
}

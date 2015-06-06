/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "init.h"
#include "motorHCSR04.h"
#include "HCSR04.h"
#include "bluetoothController.h"
#include "constants.h"


static __IO uint32_t uwTimingDelay;

 void Delay(__IO uint32_t nTime);

// **************** constants *****************************

// ********************************************************


int main(void)
{
	float measureTest;
	uint8_t cPoints;
	
	struct Message msg;
	int32_t msgTab[100];
	uint8_t ok;
	
  init();
	BC_initTMLibrary();
	msg.Args = msgTab;
	
	while(1)
	{
		cPoints = 10;
		measureTest = measureDistance();
		if(BC_isSomething())
		{
			ok = BC_readMessage(&msg);
			//if(ok)
			switch(msg.type)
			{
				case MSG_SCAN_COMMAND: 
					if(msg.cArgs == 2)
					{
						msg.cArgs = msg.Args[0];
						skanujOtoczenie(&(msg.cArgs), msg.Args[1], msg.Args);
					}
					break;
				case MSG_DRIVE_COMMAND: 
					break;
				case MSG_STEPPER_MOVE: 
					if(msg.cArgs == 3)
						sKrokObroc(msg.Args[1],msg.Args[2],msg.Args[0]);
					msg.cArgs = 0;
					msg.type = MSG_OK;
					break;
				default: 
					msg.cArgs = 0;
					msg.type = MSG_NOK;
					break;
			}
			BC_sendMessage(&msg);
		}
		Delay(50);
	}
}

// ***************** Interupts ***************************************

// **************** Update TIM2 ***********************
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
	}
}

void EXTI0_IRQHandler(void) 
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) 
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
	}
}
// *****************************************************


// *******************************************************************

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10x milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}


void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
		
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

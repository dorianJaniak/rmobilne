#include "main.h"
#include "motorHCSR04.h"
#include "constants.h"
#include "HCSR04.h"

const float sKrokKat = 0.087890625f;        // Ile stopni / krok silnika

uint8_t sKrokSterowanie [][4] = {  {1, 0, 0, 1},
                                {1, 0, 0, 0},
                                {1, 1, 0, 0},
                                {0, 1, 0, 0},
                                {0, 1, 1, 0},
                                {0, 0, 1, 0},
                                {0, 0, 1, 1},
                                {0, 0, 0, 1} };
       
void skanujOtoczenie(uint16_t * cArgs, float vKatNaS, int32_t * tab)
{
	float interwal = SK_RANGE_DEGREES / ((*cArgs)-1);
	float zakres = SK_RANGE_DEGREES / 2.0;
	float i;
	uint8_t oKrok;
	*cArgs = 0;
	for(i = -zakres; i<=zakres; i = i + interwal)
	{
		tab[(*cArgs)*2] = i;
		tab[(*cArgs)*2+1] = measureDistance();
		oKrok = 0;
		if(i+interwal <= zakres)
		{
			sKrokObroc(interwal,vKatNaS,1);
			(*cArgs)++;
			oKrok = 1;
		}
		Delay(5);
	}
	for(i = zakres; i>=-zakres; i = i - interwal)
	{
		if( i - interwal >= -zakres)
			sKrokObroc(interwal,vKatNaS, 0);
		Delay(2);
	}
	//sKrokObroc((*cArgs)*interwal,vKatNaS,1);
	if(oKrok == 0)
		*cArgs = (*cArgs)*2+2;
	else
		*cArgs = (*cArgs)*2;
	//Delay(10);
}								
                         
void sKrokObroc(float kat, float vKatNaS, short kierunek)
{
	static int ostatniPodKrok = 0;
	int przerwaMS, krokow;
	//uint32_t start;
	if(vKatNaS > 8.0f) vKatNaS = 8.0f;
	
	przerwaMS = (int)(1000.0f/(vKatNaS * 11.38f));
	krokow = (int)(kat/sKrokKat);
	if(kierunek == 1)
		while(krokow>0)
		{
			while(ostatniPodKrok <= 7 && krokow > 0)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_6, sKrokSterowanie[ostatniPodKrok][0] == 1? Bit_SET : Bit_RESET);
				GPIO_WriteBit(GPIOC,GPIO_Pin_7, sKrokSterowanie[ostatniPodKrok][1] == 1? Bit_SET : Bit_RESET);
				GPIO_WriteBit(GPIOC,GPIO_Pin_8, sKrokSterowanie[ostatniPodKrok][2] == 1? Bit_SET : Bit_RESET);
				GPIO_WriteBit(GPIOC,GPIO_Pin_9, sKrokSterowanie[ostatniPodKrok][3] == 1? Bit_SET : Bit_RESET);
				TIM_SetCounter(TIM2, 0);
			//	start = TIM_GetCounter(TIM2);
			//	while(TIM_GetCounter(TIM2) - start < 5000);
				Delay((unsigned)przerwaMS/10);
				//Delay(100);
				ostatniPodKrok++;
				krokow--; 
			}
			ostatniPodKrok = 0;
		}
	else
		while(krokow>0)
		{
			while(ostatniPodKrok >= 0 && krokow > 0)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_6, sKrokSterowanie[ostatniPodKrok][0] == 1? Bit_SET : Bit_RESET);
				GPIO_WriteBit(GPIOC,GPIO_Pin_7, sKrokSterowanie[ostatniPodKrok][1] == 1? Bit_SET : Bit_RESET);
				GPIO_WriteBit(GPIOC,GPIO_Pin_8, sKrokSterowanie[ostatniPodKrok][2] == 1? Bit_SET : Bit_RESET);
				GPIO_WriteBit(GPIOC,GPIO_Pin_9, sKrokSterowanie[ostatniPodKrok][3] == 1? Bit_SET : Bit_RESET);
				TIM_SetCounter(TIM2, 0);
			//	start = TIM_GetCounter(TIM2);
			//	while(TIM_GetCounter(TIM2) - start < 5000);
				Delay((unsigned)przerwaMS/10);
				//Delay(100);
				ostatniPodKrok--;
				krokow--; 
			}
			ostatniPodKrok = 7;
		}
}

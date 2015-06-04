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
       
void skanujOtoczenie(uint8_t * cPoints, float vKatNaS, int32_t * tab)
{
	float interwal = SK_RANGE_DEGREES / (*cPoints);
	float zakres = SK_RANGE_DEGREES / 2.0;
	float i;
	*cPoints = 0;
	for(i = -zakres; i<zakres; i = i + interwal)
	{
		tab[(*cPoints)*2] = i;
		tab[(*cPoints)*2+1] = measureDistance();
		
		sKrokObroc(interwal,vKatNaS,0);
		(*cPoints)++;
	}
	sKrokObroc(interwal*((*cPoints)-1),vKatNaS,1);
}								
                         
void sKrokObroc(float kat, float vKatNaS, short kierunek)
{
	static int ostatniPodKrok = 0;
	int przerwaMS, krokow;
	//uint32_t start;
	if(vKatNaS > 45.0f) vKatNaS = 45.0f;
	
	przerwaMS = (int)(1000.0f/(vKatNaS * 11.38f));
	krokow = (int)(kat/sKrokKat)+ostatniPodKrok;
	if(kierunek == 1)
		while(krokow>0)
		{
			while(ostatniPodKrok != 8 && krokow > 0)
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

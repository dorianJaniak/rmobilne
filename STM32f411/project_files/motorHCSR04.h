#ifndef MOTOR_HCSR04

#define MOTOR_HCSR04

void skanujOtoczenie(uint8_t * cPoints, float vKatNaS, int32_t * tab);
void sKrokObroc(float kat, float vKatNaS, short kierunek);

#endif

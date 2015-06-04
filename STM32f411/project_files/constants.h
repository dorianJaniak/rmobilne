#ifndef CONSTANTS_MODULE
#define CONSTANTS_MODULE
	
	#define MSG_OK 0							//Potwierdzenie, ze wszystko OK
	#define MSG_START 1						//Start programu
	#define MSG_SCAN_COMMAND 2		//Zadanie wykonania skanowania
	#define MSG_SCAN_RESULT 3			//Wynik skanowania
	#define MSG_STEPPER_MOVE 4		//Przydatne do kalibracji - poruszenie silnikiem krokowym
	#define MSG_DRIVE_COMMAND 5		//Porusz robotem komenda
	#define MSG_DRIVE_RESULT 6		//Wynik przemieszczenia robota
	#define MSG_NOK 99						//Operacja sie nie powiodla

	#define SK_RANGE_DEGREES 180.0	//Zakres katowy w jakim ma byc wykonany pomiar

#endif

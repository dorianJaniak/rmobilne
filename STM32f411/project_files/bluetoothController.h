#ifndef BC_MODULE
#define BC_MODULE

#include "stm32f4xx.h"

	struct Message {
		uint8_t type;
		uint16_t cArgs;
		int32_t * Args;
	};

	//Koniecznie na starcie wywolac
	void BC_initTMLibrary(void);
	void BC_sendMessage(const struct Message * msg );
	uint8_t BC_readMessage(struct Message * msg);
	// BC_interpretMessage(struct Message * msg);
	uint8_t BC_isSomething(void);

#endif

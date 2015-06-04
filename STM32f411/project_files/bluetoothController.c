
#include "bluetoothController.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_usart.h"
#include <stdio.h>
#include "constants.h"

const char BC_newLineChar[2] = { '\n', '\r' };
const char BC_seperator = ';';
uint8_t newLineType = 2;			//Gdy 1 to Linux, gdy 2 to Windows

void BC_initTMLibrary(void)
{
	struct Message startMsg;
	TM_USART_Init(USART1, TM_USART_PinsPack_1,9600);		//PA9, PA10
	//TM_USART_Puts(USART1, "W1");					//Wiadomosc startu transmisji
	startMsg.Args=NULL; startMsg.cArgs=0; startMsg.type = MSG_START;
	BC_sendMessage(&startMsg);
	
}

uint8_t BC_isSomething()
{
	if(TM_USART_BufferEmpty(USART1) != 0)
		return 0;
	else
		return 1;
}

void copyFromTabToTab(char * fromTab, uint16_t fromInd, char * toTab, uint16_t sLength)
{
	uint16_t i;
	for(i = 0; i<sLength; i++)
			toTab[i+fromInd] = fromTab[i];
}

void BC_sendMessage(const struct Message * msg )
{
	char msgd[150];
	char buffer[20];
	uint16_t cInd;
	uint16_t spLength;
	uint16_t i;
	
	cInd = 0;
	cInd += sprintf(msgd,"W%u%c%d",msg->type,BC_seperator,msg->cArgs);
	for( i= 0; i<msg->cArgs; i++)
	{
		spLength = sprintf(buffer, "%c%d", BC_seperator, msg->Args[i] );
		copyFromTabToTab(buffer, cInd, msgd, spLength);
		cInd += spLength;
	}
	do
	{
		TM_USART_Send(USART1,(uint8_t*)msgd,cInd);
		for(i=0; i<newLineType; i++)
			TM_USART_Putc(USART1,BC_newLineChar[i]);
	}while(0);
}

int32_t BC_readValue(uint8_t * ok, uint8_t iFC)
{
	int32_t valueI;
	uint8_t value[20];
	uint8_t ch;
	uint8_t cInd;
	for(cInd=0; cInd<20; cInd++)
		value[cInd] = 0;
	ch = '0';
	cInd = 0;
	if(iFC)
		TM_USART_Getc(USART1);
	do
	{
		ch = TM_USART_Getc(USART1);
		value[cInd] = ch;
		cInd++;
	}while( ((ch >= '0' && ch <= '9') || ch == '.' || ch == '-') && BC_isSomething() );
		
	if(sscanf(value,"%d",&valueI)<1)
		*ok = 0;
	else
		*ok = 1;
	return valueI;
}

//jesli ok == 1 to ok
uint8_t BC_readMessage(struct Message * msg)
{
	uint8_t ok;
	uint8_t okTemp;
	uint8_t cInd;
	char character;
	ok = 1;
	do{
		character = TM_USART_Getc(USART1);
	}while(BC_isSomething() && character != 'W');
	msg->type = BC_readValue(&okTemp,0);
	ok = ok & okTemp;
	msg->cArgs = BC_readValue(&okTemp,0);
	ok = ok & okTemp;
	for(cInd = 0; cInd < msg->cArgs; cInd++)
	{
		msg->Args[cInd] = BC_readValue(&okTemp,0);
		ok = ok & okTemp;
	}
	TM_USART_ClearBuffer(USART1);
	return ok; 
}

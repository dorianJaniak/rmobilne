#include "mbed.h"

Ticker silnikKrokTimer;
Ticker bUARTTimer;

DigitalOut led1(LED1);
DigitalOut sKrok1(PC_6);
DigitalOut sKrok2(PC_7);
DigitalOut sKrok3(PC_8);
DigitalOut sKrok4(PC_9);
Serial bUART(PA_9,PA_10);  //Bluetooth UART(TX,RX)


const double sKrokKat = 0.087890625;        // Ile stopni / krok silnika

bool sKrokSterowanie [][4] = {  {1, 0, 0, 1},
                                {1, 0, 0, 0},
                                {1, 1, 0, 0},
                                {0, 1, 0, 0},
                                {0, 1, 1, 0},
                                {0, 0, 1, 0},
                                {0, 0, 1, 1},
                                {0, 0, 0, 1} };
       
        
                         
void sKrokObroc(double kat, double vKatNaS)
{
    led1 = led1 | true;
    if(vKatNaS > 45.0) vKatNaS = 45.0;
    static int ostatniPodKrok = 0;
    int przerwaMS = (int)(1000.0/(vKatNaS * 11.38));
    int krokow = (int)(kat/sKrokKat)+ostatniPodKrok;
    while(krokow>0)
    {
        while(ostatniPodKrok != 8 && krokow > 0)
        {
            sKrok1 = sKrokSterowanie[ostatniPodKrok][0];
            sKrok2 = sKrokSterowanie[ostatniPodKrok][1];
            sKrok3 = sKrokSterowanie[ostatniPodKrok][2];
            sKrok4 = sKrokSterowanie[ostatniPodKrok][3];   
            wait_ms(przerwaMS); 
            ostatniPodKrok++;
            krokow--; 
        }
        ostatniPodKrok = 0;
    }   
    led1 = false;
}

void toggle_led() {
    sKrokObroc(10.0, 8);
}

void bUARTReakcja()
{   
    while(bUART.readable())
    {
        uint8_t znak = bUART.getc();
        if(znak == 'a')
            sKrokObroc(10.0,5);
        else if(znak == 'b')
            sKrokObroc(10.0,30);
        bUART.putc(znak);  
    }
}



void inicjalizuj()
{
    bUART.baud(9600);
    bUART.format();
    wait_ms(5000);
    bUART.printf("Jestem Robotem mobilnym Doriana i Marcina\r\nGotow do pracy!\r\n");
    bUART.attach(&bUARTReakcja, Serial::RxIrq);
   // silnikKrokTimer.attach(&toggle_led, 2);
    
}      

int main() {
    inicjalizuj();
    while (true) {
        led1 = true;
        wait_ms(1000);
        led1 = false;
        wait_ms(1000);
    }
}
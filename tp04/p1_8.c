#include <detpic32.h>



void delay(unsigned int ms)
{
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

int main(void){
    static const int diplay7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    TRISB = TRISB | 0x000F;
    TRISB = TRISB & 0x80FF;  
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
    
    while(1){
        char dip = PORTB & 0X000F;
        LATB = (LATB & 0X00FF) | (diplay7Scodes[dip] << 8);

    }
    return 0;

}

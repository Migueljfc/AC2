#include<detpic32.h>

void delay(unsigned int ms)
{
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

int main(void){
    unsigned char segment;
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    int i;
    while(1){
        LATDbits.LATD6 = !LATDbits.LATD6;
        LATDbits.LATD5 = !LATDbits.LATD5;
        segment = 1;
        for(i = 0;i<7;i++){
            LATB = (LATB & 0x80FF) | segment << 8 ;
            delay(500);
            segment = segment << 1;
        }
    }
    return 0;

}

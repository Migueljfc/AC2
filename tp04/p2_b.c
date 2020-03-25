#include <detpic32.h>


void delay(unsigned int ms)
{
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
    unsigned char digit_high = value >> 4;
    unsigned char digit_low = value & 0x0F;
    static char displayFlag = 0;
    
    if(displayFlag == 1){
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    }
    else{
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    }
    displayFlag = !displayFlag;
}

int main(void){
    TRISD = TRISD & 0xFF9F;
    LATD = LATD & 0xF9FF;

    TRISB = TRISB & 0x80FF;
    LATB = LATB & 0x80FF;

    int i = 0;
    int counter = 0;
    while(1){
        for(i = 0; i < 20; i++) {
            send2displays(counter);
            delay(10);
        }
        counter++;
    }
    
    return 0;
}


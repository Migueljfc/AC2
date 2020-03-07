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
    LATE = LATE & 0xFFF0;
    TRISE = TRISE & 0xFFF0;
    int n = 0;
    while(1){
        delay(250);
        LATE = n++ ;
    }
    return 0;
}

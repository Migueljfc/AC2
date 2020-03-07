#include <detpic32.h>


void delay(unsigned int ms)
{
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 


int main(void)
{
    LATEbits.LATE0 = 0;     // The initial value should be set
                            // before configuring the port as output
    TRISEbits.TRISE0 = 0;  // RD0 configured as output
    while(1)
    {
        delay(1000);         // half period = 0.5s
        LATEbits.LATE0 = !LATEbits.LATE0;
     }
    return 0;
}

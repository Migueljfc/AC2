#include <detpic32.h>

static int i = 0;
void _int_(12) isr_T3(void){
    if (i == 0) {
        putChar('.');
    }
    IFS0bits.T3IF = 0;
    i = !i;
}
void main(void){   
    T3CONbits.TCKPS = 7;
    PR3 = 39062 ;
    TMR3 = 0;
    
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

    T3CONbits.TON = 1;

    EnableInterrupts();
    while(1);

}
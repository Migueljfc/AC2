#include<detpic32.h>


void setPWM(unsigned int dutyCycle){
    //duty_cycle must be in range [0, 100]
    OC1RS = (int)500 * dutyCycle;
} 

void _int_(12) isr_T3(void){
    IFS0bits.T3IF = 0;
}
void main(void){
    TRISDbits.TRISD0=0;

    T3CONbits.TCKPS = 5; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR3 = 49999; 
    TMR3 = 0; // Reset timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
    // timer configuration sequence)
    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1 ;// Use timer T3 as the time base for PWM generation
    setPWM(100);
    OC1CONbits.ON = 1; // Enable OC1 module 

    LATDbits.LATD0 = 0;
    EnableInterrupts();
}


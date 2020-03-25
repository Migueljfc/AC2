#include <detpic32.h>

volatile unsigned char voltage = 0;

unsigned char toBcd(unsigned char value)
{
return ((value / 10) << 4) + (value % 10);
}

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
void _int_(27) isr_adc(void){
    int soma = 0;
    int *p = (int *)(&ADC1BUF0);
    for(;p < (int *)(&ADC1BUF8); p+= 4){
        soma += *p;
    }
    double media = soma / 8;
    voltage = (char)((media*33)/1023);
    IFS1bits.AD1IF = 0;

}


void main(void){
     TRISBbits.TRISB4 = 1;       // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0;       // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;       // Conversion trigger selection bits: in this
                                // mode an internal counter ends sampling and
                                // starts conversion
    AD1CON1bits.CLRASAM = 1;    // Stop conversions when the 1st A/D converter
                                // interrupt is generated. At the same time,
                                // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;      // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 8-1;       // Interrupt is generated after XX samples
                                // (replace XX by the desired number of
                                // consecutive samples)
    AD1CHSbits.CH0SA = 4;       // replace x by the desired input
                                // analog channel (0 to 15)
    AD1CON1bits.ON = 1;         // Enable A/D converter
                                // This must the last command of the A/D
                                // configuration sequence

    IPC6bits.AD1IP = 2;         //configure priority of A/D interrupys to 2
    IEC1bits.AD1IE = 1;         //enable A/D interrupts
    IFS1bits.AD1IF = 0;          //clear A/D interrupt flag
    
    TRISD = TRISDbits.TRISD5 = 0;
    TRISD = TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x80FF;
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    LATB = LATB & 0x80FF;
    
    IFS1bits.AD1IF = 0;          // Reset AD1IF flag
    EnableInterrupts();         // Global Interrupt Enable 
    int i = 0;
    while(1){
        delay(10);
        if((i++) == 25){
            AD1CON1bits.ASAM = 1; // start A/D conversion
            i = 0;
        }
        send2displays(toBcd(voltage & 0xFF));
    }
}

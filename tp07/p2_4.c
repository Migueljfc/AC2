#include <detpic32.h>

volatile int voltage;

void setPWM(unsigned int dutyCycle){
    //duty_cycle must be in range [0, 100]
    OC1RS = (int) 500 * dutyCycle;
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

void _int_(4) isr_T1(void){
    AD1CON1bits.ASAM = 1;
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
    IFS0bits.T3IF = 0;
}
void configureAll()
{
    TRISDbits.TRISD0=0;
    TRISBbits.TRISB4 = 1;     // RBx digital output disconnected
    AD1PCFGbits.PCFG4 = 0;    // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;     // Conversion trigger selection bits: in this
                              // mode an internal counter ends sampling and
                              // starts conversion
    AD1CON1bits.CLRASAM = 1;  // Stop conversions when the 1st A/D converter
                              // interrupt is generated. At the same time,
                              // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 8 - 1; // Interrupt is generated after XX samples
                              // (replace XX by the desired number of
                              // consecutive samples)
    AD1CHSbits.CH0SA = 4;     // replace x by the desired input
                              // analog channel (0 to 15)
    AD1CON1bits.ON = 1;       // Enable A/D converter
                              // This must the last command of the A/D
                              // configuration sequence

    IPC6bits.AD1IP = 2; //configure priority of A/D interrupys to 2
    IEC1bits.AD1IE = 1; //enable A/D interrupts
    IFS1bits.AD1IF = 0; //clear A/D interrupt flag


    T1CONbits.TCKPS = 3;
    PR1 = 19530;
    TMR1 = 0;
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;

    T3CONbits.TCKPS = 2;
    PR3 = 49999;
    TMR3 = 0;
    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

    OC1CONbits.OCM = 6;
    OC1CONbits.OCTSEL = 1;
    OC1CONbits.ON = 1;
    LATDbits.LATD0 = 0;

    EnableInterrupts(); // Global Interrupt Enable
    T1CONbits.TON = 1;
    T3CONbits.TON = 1;


}


void main(void)
{
    TRISB = ((TRISB & 0xFFFC)| 0X0003);
    int dutyCycle;
    configureAll();
    while (1)
    {
        char portVal = PORTB & 0x0003;
        switch (portVal)
        {
        case 0: // Measure input voltage
            IEC0bits.T1IE = 1;// Enable T1 interrupts
            setPWM(0); // LED OFF
            printInt(OC1RS,10);
            putChar('\n');
            break;
        case 1: // Freeze
            IEC0bits.T1IE = 0;// Disable T1 interrupts
            setPWM(100); // LED ON (maximum bright)
            printInt(OC1RS,10);
            putChar('\n');
            break;
        default: // LED brigthness control
            IEC0bits.T1IE = 1;// Enable T1 interrupts
            dutyCycle = voltage * 3;
            setPWM(dutyCycle);
            printInt(OC1RS,10);
            putChar('\n');
            break;
        }
    }
}

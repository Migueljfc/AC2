#include <detpic32.h>
#include "i2c.c"

volatile int temperature;

int getTemperature(){
    int ack;
    i2c1_start();
    ack = i2c1_send(ADDR_WR);
    ack += i2c1_send(RTR);
    i2c1_start();
    ack += i2c1_send(ADDR_RD);
    if(ack != 0){
        return 1000;
    } 
    temperature = i2c1_receive(I2C_NACK);
    i2c1_stop();
    return ack;
} 
unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value) {
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 1;
    unsigned char dh = display7Scodes[value >> 4];
    unsigned char dl = display7Scodes[value & 0x0F];

    if(displayFlag) {
       
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB = (dh << 8) | (LATB & 0x00FF);
    }

    else {
       
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB = (dl << 8) | (LATB & 0x00FF);
    }

    displayFlag = !displayFlag;
}





void _int_(4) isr_T1(void){
    int t;
    t = getTemperature();
    printInt10(temperature);
    putChar('\n');
    if(t == 1000){
        i2c1_stop();
        printStr("ERRO\n");
    }
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
    send2displays(toBcd((char) temperature & 0xFF));
    IFS0bits.T3IF = 0;
}
void configureAll()
{
    
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


int main(void)
{
    TRISB = ((TRISB & 0x80FF)| 0X0003);
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
    LATB = ((LATB & 0x80FF)| 0X0003);
    configureAll();
    i2c1_init(TC74_CLK_FREQ);
    while (1)
    {
        if(PORTBbits.RB0 == 1 && PORTBbits.RB1 == 1){
            IEC0bits.T1IE  = 1;
        }
        else{
            IEC0bits.T1IE = 0;
        }
    }
    return 0;
}

#include <detpic32.h>

void configUart(int baudRate, char paridade, int nBits, int stopBit)
{
    U1MODEbits.BRGH = 0;
    U1BRG = ((PBCLK + 8 * baudRate) / (16 * baudRate)) - 1;
    if (paridade == 'N')
    {
        U1MODEbits.PDSEL = 0b00;
    }
    else if (paridade == 'E')
    {
        U1MODEbits.PDSEL = 0b01;
    }
    else if (paridade == 'O')
    {
        U1MODEbits.PDSEL = 0b10;
    }

    if (stopBit == 1)
    {
        U1MODEbits.STSEL = 0;
    }
    if (stopBit == 2)
    {
        U1MODEbits.STSEL = 1;
    }
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
}
void delay(int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000);
    }
}
void putc(char byte2send)
{
    while (U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

int main(void)
{
    configUart(115200, 'N', 8, 1);
    while (1)
    {
        putc('+');
        delay(1000);
    }
}

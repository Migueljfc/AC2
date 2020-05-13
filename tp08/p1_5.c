#include <detpic32.h>

void configUart(int baudRate, char paridade,int stopBit)
{
    if (baudRate < 600 || baudRate > 115200)
    {
        baudRate = 115200;
    }
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
    else
    {
        U1MODEbits.PDSEL = 0b00;
    }

    if (stopBit == 1)
    {
        U1MODEbits.STSEL = 0;
    }
    if (stopBit == 2)
    {
        U1MODEbits.STSEL = 1;
    }
    else
    {
        U1MODEbits.STSEL = 0;
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
        while (readCoreTimer() < 20000)
            ;
    }
}
void putc(char byte2send)
{
    while (U1STAbits.UTXBF == 1)
        ;
    U1TXREG = byte2send;
}

void puts(char *str)
{
    while (*str != '\0')
    {
        putc(*str);
        *str++;
    }
}

int main(void)
{
    configUart(19200, 'N', 1);
    while (1)
    {
        puts("String de teste\n");
        delay(1000);
    }
}

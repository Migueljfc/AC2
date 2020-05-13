#include <detpic32.h>

int main(void)
{
    int baudRate = 115200;
    U1BRG = (PBCLK / (16 * baudRate)) - 1;
    U1MODEbits.BRGH = 0;
    U1MODEbits.STSEL = 0;
    U1MODEbits.PDSEL = 0b00;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;

    return 0;
}

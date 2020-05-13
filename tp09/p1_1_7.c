#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // Transmission buffer
volatile circularBuffer rxb; // Reception buffer

void comDrv_config(unsigned int baudrate, char parity, unsigned int stopbits)
{
    if (baudrate < 600 || baudrate > 115200)
        baudrate = 115200;
    U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;
    U1MODEbits.BRGH = 0; // 16

    switch (parity)
    {
    case 'E':
        U1MODEbits.PDSEL = 1;
        break;

    case 'O':
        U1MODEbits.PDSEL = 2;
        break;

    default:
        U1MODEbits.PDSEL = 0;
        break;
    }

    switch (stopbits)
    {
    case 2:
        U1MODEbits.STSEL = 1;
        break;

    default:
        U1MODEbits.STSEL = 0;
        break;
    }

    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;

    IPC6bits.U1IP = 2;
    IEC0bits.U1RXIE = 1;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1TXIE = 1;
    IFS0bits.U1TXIF = 0;
}

void comDrv_flushRx(void)
{
    rxb.count = 0;
    rxb.head = 0;
    rxb.tail = 0;
}

void comDrv_flushTx(void)
{
    txb.count = 0;
    txb.head = 0;
    txb.tail = 0;
}

void comDrv_putc(char ch)
{
    while (txb.count == BUF_SIZE){}         // Wait while buffer is full
    txb.data[txb.tail] = ch;                // Copy character to the transmission buffer at position "tail"
    txb.tail = (txb.tail + 1) & INDEX_MASK; // Increment "tail" index (mod. BUF_SIZE)
    DisableUart1TxInterrupt();              // Begin of critical section
    txb.count++;                            //Increment "count" variable
    EnableUart1TxInterrupt();                //End of critical section
}

void comDrv_puts(char *s)
{
    while (*s != '\0')
    {
        comDrv_putc(*s);
        s++;
    }
}

void _int_(24) isr_uart(void)
{
    if (IFS0bits.U1TXIF)
    {
        if (txb.count > 0)
        {
            U1TXREG = txb.data[txb.head];           // Copy character pointed by "head" to U1TXREG register
            txb.head = (txb.head + 1) & INDEX_MASK; // Increment "head" variable (mod BUF_SIZE)
            txb.count--;                            // Decrement "count" variable
        }
        if (txb.count == 0)
        {
            DisableUart1TxInterrupt();
        }
        IFS0bits.U1TXIF = 0; // Reset UART1 TX interrupt flag
    }
}

int main(void)
{
    comDrv_config(115200, 'N', 1); // default "pterm" parameters with TX and RX interrupts disabled
    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();
    while (1)
    {
        comDrv_puts("Teste do bloco de transmissao do device driver!...\n");
    }
    return 0;
}

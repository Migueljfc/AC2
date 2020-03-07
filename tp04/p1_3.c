#include <detpic32.h>
int main(void){
    LATB = LATB & 0x00FF;
    LATD = LATD & 0xFF9F;
    TRISB = TRISB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    while(1){
        char c = getChar();
        putChar(c);
        putChar('\r');
        switch (c)
        {
            case 'a' :
                LATB = LATB & 0x00FF;
                LATBbits.LATB8 = 1;
            break;
            case 'b' :
                LATB = LATB & 0x00FF;
                LATBbits.LATB9 = 1;
            break;
            case 'c' :
                LATB = LATB & 0x00FF;
                LATBbits.LATB10 = 1;
            break;
            case 'd' :
                LATB = LATB & 0x00FF;
                LATBbits.LATB11 = 1;
            break;
            case 'e' :
                LATB = LATB & 0x00FF;
                LATBbits.LATB12 = 1;
            break;
            case 'f' :
                LATB = LATB & 0x00FF;
                LATBbits.LATB13 = 1;
            break;
            case 'g' :
                LATB = LATB & 0x00FF;
                LATBbits.LATB14 = 1;
            break;
            case '.':
                LATB = LATB & 0x00FF;
                LATBbits.LATB15 = 1;
            break;
            case  'A':
                LATB = LATB & 0x00FF;
                LATBbits.LATB8 = 1;
            break;
            case  'B':
                LATB = LATB & 0x00FF;
                LATBbits.LATB9 = 1;
            break;
            case  'C':
                LATB = LATB & 0x00FF;
                LATBbits.LATB10 = 1;
            break;
            case  'D':
                LATB = LATB & 0x00FF;
                LATBbits.LATB11 = 1;
            break;
            case 'E':
                LATB = LATB & 0x00FF;
                LATBbits.LATB12 = 1;
            break;
            case  'F':
                LATB = LATB & 0x00FF;
                LATBbits.LATB13 = 1;
            break;
            case  'G':
                LATB = LATB & 0x00FF;
                LATBbits.LATB14 = 1;
            break;
            default:

            break;
        }
    }
    return 0;
}

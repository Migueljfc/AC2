#include <detpic32.h>


#define WRITE 0x02
#define RDSR 0x05
#define READ 0x03
#define WRSR 0x01
#define WRDI 0x04
#define WREN 0x06
#define EEPROM_CLOCK 500000

void spi2_setClock(unsigned int clock_freq){
    SPI2BRG = (PBCLK + clock_freq) / (2 * clock_freq) - 1;
}

void spi2_init(void){
   volatile char trash ;
    SPI2CONbits.ON = 0; // Disable SPI2 module
    SPI2CONbits.CKP = 0; // Configure clock idle state as logic level 0
    SPI2CONbits.CKE = 1; // Configure the clock active transition: from active state to idle state (1 > 0)
    SPI2CONbits.SMP = 0; // Configure SPI data input sample phase bit (middle of data output time)
    SPI2CONbits.MODE32 = 0;
    SPI2CONbits.MODE16 = 0; // Configure word length (8 bits)
    SPI2CONbits.ENHBUF = 1; // Enable Enhanced buffer mode (this allows the usage of FIFOs RX,TX) 
    SPI2CONbits.MSSEN = 1; // Enable slave select support (Master Mode Slave Select) 
    SPI2CONbits.MSTEN = 1; // Enable master mode
    while(SPI2STATbits.SPIRBE == 0){// Clear RX FIFO: 
        trash = SPI2BUF;            // FIFO (discard data) 
    }
    SPI2STATbits.SPIROV = 0;   // Clear overflow error flag 
    SPI2CONbits.ON = 1; // Enable SPI2 module
}

char eeprom_readStatus(void){
    volatile char trash;
    while(SPI2STATbits.SPIRBE == 0){// Clear RX FIFO: 
        trash = SPI2BUF;            // FIFO (discard data) 
    }
    SPI2STATbits.SPIROV = 0;   // Clear overflow error flag 
    SPI2BUF = RDSR; // Send RDSR command
    SPI2BUF = 0; //  Send anything so that EEPROM clocks data into SO 
    while(SPI2STATbits.SPIBUSY); // wait while SPI module is working 
    trash = SPI2BUF; // First char received is garbage (received while sending command) 
    return SPI2BUF;  // Second received character is the STATUS value 
}


void eeprom_writeStatusCommand(char command){
    while(eeprom_readStatus() & 0x01);  // Wait while WIP is true 
    SPI2BUF = command;                  // Copy "command" value to SPI2BUF (TX FIFO) 
    while(SPI2STATbits.SPIBUSY);        // Wait while SPI module is working (SPIBUSY set)
}

int main(void){
    char status;
    spi2_init();
    spi2_setClock(EEPROM_CLOCK);
    eeprom_writeStatusCommand(WREN);
    for(;;){
        status = eeprom_readStatus();
        printInt(status, 2 | 4 << 16);
    }
}

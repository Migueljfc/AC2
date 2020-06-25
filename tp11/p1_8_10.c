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

void eeprom_writeData(int address, char value){
    address = address & 0x01FF;   // Apply a mask to limit address to 9 bits 
    while(eeprom_readStatus() & 0x01);  // Read STATUS and wait while WIP is true (write in progress) 
    eeprom_writeStatusCommand(WREN);   // Read STATUS and wait while WIP is true (write in progress) Enable write operations (activate WEL bit in STATUS register, using eeprom_writeStatusCommand() function ) 
    SPI2BUF = WRITE | ((address & 0x100) >> 5);   // Copy WRITE command and A8 address bit to the TX FIFO
    SPI2BUF = address & 0x00FF;  // Copy address (8 LSBits) to the TX FIFO 
    SPI2BUF = value;    // Copy "value" to the TX FIFO 
    while(SPI2STATbits.SPIBUSY);    // Wait while SPI module is working (SPIBUSY) 

}

char eeprom_readData(int address){
    volatile char trash;
    while(SPI2STATbits.SPIRBE == 0){// Clear RX FIFO: 
        trash = SPI2BUF;            // FIFO (discard data) 
    }
    SPI2STATbits.SPIROV = 0;   // Clear overflow error flag 
    address = address & 0x01FF;   // Apply a mask to limit address to 9 bits
    while(eeprom_readStatus() & 0x01);  // Read STATUS and wait while WIP is true (write in progress) 
    SPI2BUF = READ | ((address & 0x100) >> 5); // Copy READ command and A8 address bit to the TX FIFO 
    address = address & 0x00FF; // Copy address (8 LSBits) to the TX FIFO 
    SPI2BUF = SPI2BUF & 0x00; 
    while(SPI2STATbits.SPIBUSY);    // Wait while SPI module is working (SPIBUSY) 
    trash = SPI2BUF;    // Read and discard 2 characters from RX FIFO (use "trash" variable) 
    trash = SPI2BUF;
    return SPI2BUF; // Read RX FIFO and return the corresponding value 
}

int main(void){
    spi2_init();
    spi2_setClock(EEPROM_CLOCK);
    eeprom_writeStatusCommand(WREN);
    printStr("Digite R para ler o endereço e imprimir o valor lido da memória ou W para escrever na EEPROM um valor num endereço de memória\n");
    for(;;){
        char c = getChar();
        if(c == 'R'){
            printStr("Endereço:\n");
            int address = readInt10();
            char value = eeprom_readData(address);
            printStr("Valor lido na memoria:\n");
            putChar(value);
            printStr("--------------------\n");
        }
        else if(c == 'W'){
            printStr("Endereço:\n");
            int address = readInt10();
            printStr("Valor a escrever na memória:\n");
            char value = getChar();
            eeprom_writeData(address,value);
            printStr("---------------------------\n");
        }
        else{
            printStr("Caracter Inválido\n");
        }
        printStr("Digite R para ler o endereço e imprimir o valor lido da memória ou W para escrever na EEPROM um valor num endereço de memória\n");

    }
    return 0;
}

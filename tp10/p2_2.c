#include <detpic32.h>
#include "i2c.h" 

int getTemperature(int *temperature){
    int ack;
    i2c1_start();
    ack = i2c1_send(ADDR_WR);
    ack += i2c1_send(RTR);
    i2c1_start();
    ack += i2c1_send(ADDR_RD);
    if(ack != 0){
        i2c1_stop();
        printStr("ERRO\n");
        return 1000;
    } 
    *temperature = i2c1_receive(I2C_NACK);
    i2c1_stop();
    return ack;
}

int main(void){
    i2c1_init(TC74_CLK_FREQ);
    int temperature,t;
    while(1){
        t = getTemperature(&temperature);
        if(t == 1000){
            break;
        }
        printInt10(temperature);
        putChar('\n');
        delay(250);
    }
    return 0;
}

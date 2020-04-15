#include <detpic32.h>

void setPWM(unsigned int dutyCycle){
    //duty_cycle must be in range [0, 100]
    OC1RS =(int) 500 * dutyCycle;
}

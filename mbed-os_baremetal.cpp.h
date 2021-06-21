// mbed-os_baremetal.cpp

#include "mbed.h"

CAN can(PD_0, PD_1);
DigitalOut pin(PG_0);
CANMessage msg;
Ticker ticker;

void send(void);

int main(void){
    printf("mbed-os-periodic-task-%s %s\r\n", __DATE__, __TIME__);
    printf("OS_STACK_SIZE:%d\r\n", OS_STACK_SIZE);

    ticker.attach(callback(send), 5ms);

    while(1){
        
    }
}


void send(void){
    msg.id = 1;
    msg.len = 8;
    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFC;
    can.write(msg);
    pin = !pin;
}
// T-motorをトルク指令で制御するプログラム
// ROSを使うと通信速度がボトルネックになるのでマイコンで実装．

#include "PinNames.h"
#include "mbed.h"
#include "basic_op.h"
#include "can_com.h"
#include "mbed_wait_api.h"

#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported for this target
# endif

// 理論値は0.76[Nm]
float tau_ref = 0.521;   // トルク指令値(mutexで管理すること！)

Thread thread;
Mutex mutex;
volatile bool running = false;

DigitalOut led(LED1);   // デバッグ用LED
DigitalIn toggle(USER_BUTTON); // スタート用のトグル

CAN can(CAN_RX_PIN, CAN_TX_PIN);   // CAN通信設定
CANMessage send_msg;


// 送信スレッド
void send_thread(void){
    mutex.lock();
    
    if(running){
        pack_cmd(&send_msg, 0.0, 0.0, 0.0, 0.0, tau_ref);
        can.write(send_msg);
    }else{
        pack_cmd(&send_msg, 0.0, 0.0, 0.0, 0.0, 0.0);
        can.write(send_msg);
    }
    mutex.unlock();
    ThisThread::sleep_for(5ms);
}


int main(){
    printf("main() start\r\n");
    can.frequency(1000000);
    led = 0;


    wait_us(1000000);
    enter_control_mode(&send_msg, CAN_MOTOR_ID);
    can.write(send_msg);
    printf("enter_control_mode\r\n");
    wait_us(10000);

    send_msg.id = CAN_MOTOR_ID;
    send_msg.len = CAN_DATA_LENGTH;

    running = true;
    thread.start(callback(send_thread));    // send_thread()の開始

    while (true) {
        led = 1;
        
        ThisThread::sleep_for(30s);
        led = 0;    // LEDを消す
        running = false;
        thread.join();  // wait for thread to terminate.
    }
    return 0;
}
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

void pack_cmd(CANMessage *msg, float p_des, float v_des, float kp, float kd, float t_ff);
void enter_control_mode(CANMessage *, uint8_t);

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


void pack_cmd(CANMessage *msg, float p_des, float v_des, float kp, float kd, float t_ff){
    p_des = fminf(fmaxf(P_MIN, p_des), P_MAX);
    v_des = fminf(fmaxf(V_MIN, v_des), V_MAX);
    kp = fminf(fmaxf(KP_MIN, kp), KP_MAX);
    kd = fminf(fmaxf(KD_MIN, kd), KD_MAX);
    t_ff = fminf(fmaxf(T_MIN, t_ff), T_MAX);
    
    // convert float -> uint
    int p_int = float_to_uint(p_des, P_MIN, P_MAX, 16);     // Position
    int v_int = float_to_uint(v_des, V_MIN, V_MAX, 12);     // Velocity
    int kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);     // Kp
    int kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);     // Kd
    int t_int = float_to_uint(t_ff, T_MIN, T_MAX, 12);      // Torque
    
    // Pack ints into the CAN buffer
    msg->data[0] = p_int >> 8;
    msg->data[1] = p_int & 0xFF;
    msg->data[2] = v_int >> 4;
    msg->data[3] = ((v_int & 0xF)<<4) | (kp_int >> 8);
    msg->data[4] = kp_int & 0xFF;
    msg->data[5] = kd_int >> 4;
    msg->data[6] = ((kd_int & 0xF)<<4) | (t_int>>8);
    msg->data[7] = t_int & 0xFF;
}

void enter_control_mode(CANMessage* msg, uint8_t id_){
    msg->id = id_;
    msg->len = CAN_DATA_LENGTH;
    msg->data[0] = 0xFF;
    msg->data[1] = 0xFF;
    msg->data[2] = 0xFF;
    msg->data[3] = 0xFF;
    msg->data[4] = 0xFF;
    msg->data[5] = 0xFF;
    msg->data[6] = 0xFF;
    msg->data[7] = 0xFC;
}

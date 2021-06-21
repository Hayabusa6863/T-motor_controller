#include "T-motor_controller.h"
#include <cstdint>

T_motor_controller::T_motor_controller(void)
    : led(LED1),
      can(can_rx_pin, can_tx_pin, can_baudrate),
      can_com(&can)
{
    // EventQueueを使ってRTOS化？
    can.attach(callback(this, &T_motor_controller::can_callback), CAN::RxIrq);
    ThisThread::sleep_for(100ms);
    add_motor();    
}


// 設定ファイルからモータのCAN_IDを読み出す
void T_motor_controller::add_motor(void){
    uint8_t id_ = 0;
    for(uint8_t i=0; i<(sizeof(motor_to_control)/sizeof(*motor_to_control)); i++){
        motor.push_back(Motor_Status(motor_to_control[i]));
        can_com.enter_control_mode(motor_to_control[i]);
        motor_id.emplace(motor_to_control[i], &motor[id_]);
        id_++;
    }
}


// CAN通信受信割込みコールバック関数
// CANMessageの送信元IDから対応したmotor_statusを呼び出したい！
void T_motor_controller::can_callback(void){
    CANMessage msg_buf;
    if(can.read(msg_buf)){
        if(msg_buf.id == CAN_HOST_ID){  // 送信先が自分かどうか判断
            can_com.unpack_reply(msg_buf, motor_id[msg_buf.id]);    // CAN_IDに対応したMotor_statusのポインタを渡す．
        }
    }
}


// CAN送信スレッド
// 
void T_motor_controller::can_send_thread(void){
    for(auto& m : motor){
        CANMessage msg;
        can_com.pack_cmd(msg, m);
        can.write(msg);
    }
}


// コントローラの有効化
void T_motor_controller::enableController(void){
    thread.start(callback(this, &T_motor_controller::can_send_thread)); // start can_send thread
    led = 1;    // Turn on LED
}


// コントローラ無効化
void T_motor_controller::disableController(void){
    thread.terminate(); // Stop can_send thread
    led = 0;    // Turn off LED
}
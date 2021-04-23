#include "T-motor_controller.h"
#include <cstdint>

T_motor_controller::T_motor_controller(void)
    : led(LED1),
      can(can_rx_pin, can_tx_pin, can_baudrate),
      can_com(&can)
{
    can.attach(callback(this, &T_motor_controller::can_callback), CAN::RxIrq);
    // add motor for all designated motors
    // for()

    // findで探索したい
    /*
    std::map<string, int> a_map;

    std::map<string, int>::iterator it = a_map.find("apple");
    if (it != a_map.end()) {
        std::cout << "apple に關聯附けられてゐたのは"  << it->second << std::endl;
    }else {
        std::cout << "apple には何も關聯附けられてゐない。" << std::endl;
    }
    */
    uint8_t id_;
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
            
        }
    }
}


// コントローラの有効化
void T_motor_controller::enableController(void){
    led = 1;    // Turn on LED
    // start can_send thread
}

// コントローラ無効化
void T_motor_controller::disableController(void){
    led = 0;
}
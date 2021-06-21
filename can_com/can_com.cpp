#include "can_com.h"
#include <cstdint>

// convert float -> uint
const int p_int = float_to_uint(0.0, P_MIN, P_MAX, 16);
const int v_int = float_to_uint(0.0, V_MIN, V_MAX, 12);
const int kp_int = float_to_uint(0.0, KP_MIN, KP_MAX, 12);
const int kd_int = float_to_uint(0.0, KD_MIN, KD_MAX, 12);


// コンストラクタ
CAN_com::CAN_com(CAN *can_pointer_)
{
    can = can_pointer_;
}


// 各種値をCANMessageに変換して格納
void CAN_com::pack_cmd(CANMessage &msg, Motor_Status m){
    int t_int = float_to_uint(fminf(fmaxf(T_MIN, m.getTargetEffort()), T_MAX), T_MIN, T_MAX, 12);      // Torque
    
    msg.id = m.getId();
    // Pack ints into the CAN buffer
    msg.data[0] = p_int >> 8;
    msg.data[1] = p_int & 0xFF;
    msg.data[2] = v_int >> 4;
    msg.data[3] = ((v_int & 0xF)<<4) | (kp_int>>8);
    msg.data[4] = kp_int & 0xFF;
    msg.data[5] = kd_int >> 4;
    msg.data[6] = ((kd_int & 0xF)<<4) | (t_int>>8);
    msg.data[7] = t_int & 0xFF;
}


// CANMessageを各種のアナログ値に分解
void CAN_com::unpack_reply(const CANMessage& msg, Motor_Status* m){
    int id = msg.data[0];   // IDの取得
    int p_int = (msg.data[1]<<8) | msg.data[2];
    int v_int = (msg.data[3]<<4) | (msg.data[4]>>4);
    int i_int = ((msg.data[4]&0xF)<<8) | msg.data[5];

    m->setPosition(uint_to_float(p_int, P_MIN, P_MAX, 16)); // 現在位置をセット
    m->setVelocity(uint_to_float(v_int, V_MIN, V_MAX, 12)); // 現在速度をセット
    m->setEffort(uint_to_float(i_int, T_MIN, T_MAX, 12));   // 現在トルクをセット
}


// 制御モードに入る
void CAN_com::enter_control_mode(uint8_t can_id_){
    CANMessage msg;
    msg.id = can_id_;
    msg.len = CAN_DATA_LENGTH;
    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFC;

    can->write(msg);
    wait_us(10000);
}


// 制御モードを抜ける
void CAN_com::exit_control_mode(uint8_t can_id_){
    CANMessage msg;
    msg.id = can_id_;
    msg.len = CAN_DATA_LENGTH;
    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFD;

    can->write(msg);
    wait_us(10000);
}


// 現在位置をゼロにリセット
void CAN_com::set_position_zero(uint8_t can_id_){
    CANMessage msg;
    msg.id = can_id_;
    msg.len = CAN_DATA_LENGTH;
    msg.data[0] = 0xFF;
    msg.data[1] = 0xFF;
    msg.data[2] = 0xFF;
    msg.data[3] = 0xFF;
    msg.data[4] = 0xFF;
    msg.data[5] = 0xFF;
    msg.data[6] = 0xFF;
    msg.data[7] = 0xFE;

    can->write(msg);
    wait_us(10000);
}
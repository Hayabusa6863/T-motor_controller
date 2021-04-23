#include "can_com.h"
#include <cstdint>

// convert float -> uint
const int p_int = float_to_uint(0.0, P_MIN, P_MAX, 16);
const int v_int = float_to_uint(0.0, V_MIN, V_MAX, 12);
const int kp_int = float_to_uint(0.0, KP_MIN, KP_MAX, 12);
const int kd_int = float_to_uint(0.0, KD_MIN, KD_MAX, 12);

CAN_com::CAN_com(CAN *can_)
{
    can = can_;
}


// 各種値をCANMessageに格納
void CAN_com::pack_cmd(CANMessage &msg, Motor_Status m){
    int t_int = float_to_uint(fminf(fmaxf(T_MIN, m.getTargetEffort()), T_MAX), T_MIN, T_MAX, 12);      // Torque
    
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


// CANMessageを各種値に分解
void CAN_com::unpack_reply(const CANMessage& msg, Motor_Status& m){
    int id = msg.data[0];   // IDの取得
    int p_int = (msg.data[1]<<8) | msg.data[2];
    int v_int = (msg.data[3]<<4) | (msg.data[4]>>4);
    int i_int = ((msg.data[4]&0xF)<<8) | msg.data[5];

    m.setPosition(uint_to_float(p_int, P_MIN, P_MAX, 16));
    m.setVelocity(uint_to_float(v_int, V_MIN, V_MAX, 12));
    m.setEffort(uint_to_float(i_int, T_MIN, T_MAX, 12));
}


// control_modeに入る
void CAN_com::enter_control_mode(uint8_t id_){
    CANMessage msg;
    msg.id = id_;
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
}


void CAN_com::exit_control_mode(uint8_t id_){
    CANMessage msg;
    msg.id = id_;
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
}


void CAN_com::set_position_zero(uint8_t id_){
    CANMessage msg;
    msg.id = id_;
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
}


// 受け取ったモータ単体のデータを処理
void CAN_com::can_callback(void){
    
}
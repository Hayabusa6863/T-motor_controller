#include "can_com.h"
#include "CAN.h"

CAN_com::CAN_com(PinName RX_, PinName TX_)
    : can(RX_, TX_)
{
    can.frequency(default_baudrate);
}


CAN_com::CAN_com(PinName RX_, PinName TX_, const int baud)
    : can(RX_, TX_, baud)
{
}

// 各種値をCANMessageに格納
void CAN_com::pack_cmd(CANMessage *msg, float p_des, float v_des, float kp, float kd, float t_ff){
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


// CANMessageを各種値に分解
void CAN_com::unpack_reply(CANMessage msg, float *pos_, float *vel_, float *tt_f_){
    int id = msg.data[0];
    int p_int = (msg.data[1]<<8) | msg.data[2];
    int v_int = (msg.data[3]<<4) | (msg.data[4]>>4);
    int i_int = ((msg.data[4]&0xF)<<8) | msg.data[5];
    
    // convert int -> float
    float p = uint_to_float(p_int, P_MIN, P_MAX, 16);
    float v = uint_to_float(v_int, V_MIN, V_MAX, 12);
    float i = uint_to_float(i_int, T_MIN, T_MAX, 12);
    
    if(id == CAN_MOTOR_ID){
        // idが合致したら
        *pos_ = p;
        *vel_ = v;
        *tt_f_ = i;
    }
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

    can.write(msg);
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

    can.write(msg);
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

    can.write(msg);
}
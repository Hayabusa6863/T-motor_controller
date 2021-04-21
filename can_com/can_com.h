#ifndef CAN_COM_H_
#define CAN_COM_H_

#include "mbed.h"
#include "CAN.h"
#include "basic_op.h"
#include <cstdint>

// 上限値の設定
#define P_MIN   -12.5f
#define P_MAX   12.5f
#define V_MIN   -45.0f
#define V_MAX   45.0f
#define KP_MIN   0.0f
#define KP_MAX   500.0f
#define KD_MIN   0.0f
#define KD_MAX   5.0f
#define T_MIN   -18.0f
#define T_MAX   18.0f

// CAN_ID設定
#define CAN_HOST_ID     0
#define CAN_MOTOR_ID    1

// NUCLEO_F446REのピン設定
#define CAN_RX_PIN  PB_8
#define CAN_TX_PIN  PB_9

#define CAN_DATA_LENGTH 8

void pack_cmd(CANMessage *msg, float p_des, float v_des, float kp, float kd, float t_ff);
void unpack_reply(CANMessage msg, float *pos_, float *vel_, float *tt_f_);

void enter_control_mode(CANMessage*, uint8_t);
void exit_control_mode(CANMessage*, uint8_t);
void set_position_zero(CANMessage*, uint8_t);

#endif
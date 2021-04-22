#ifndef CAN_COM_H_
#define CAN_COM_H_

#include "mbed.h"
#include "basic_op.h"
#include "motor_status.h"
#include "motor_config.h"
#include <cstdint>



// CAN_ID設定
#define CAN_HOST_ID     0
#define CAN_MOTOR_ID    1
#define CAN_DATA_LENGTH 8

const PinName can_rx_pin = PB_8;
const PinName can_tx_pin = PB_9;
const int can_baudrate = 1000000;


void pack_cmd(CANMessage *msg, float p_des, float v_des, float kp, float kd, float t_ff);
void unpack_reply(CANMessage msg, float *pos_, float *vel_, float *tt_f_);

void enter_control_mode(uint8_t);
void exit_control_mode(uint8_t);
void set_position_zero(uint8_t);

void can_callback(void);

#endif  // CAN_com
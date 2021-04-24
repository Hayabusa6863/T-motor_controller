#ifndef CAN_COM_H_
#define CAN_COM_H_

#include "mbed.h"
#include "basic_op.h"
#include "motor_status.h"
#include "motor_config.h"
#include <cstdint>


// CAN_ID設定
#define CAN_HOST_ID         0
#define CAN_MOTOR_ID        1
#define CAN_DATA_LENGTH     8

class CAN_com{
private:
    CAN *can;
    const int default_baudrate = 1000000;

    

public:
    CAN_com(CAN *can_);
    void pack_cmd(CANMessage&, Motor_Status);
    void unpack_reply(const CANMessage&, Motor_Status*);
    
    void enter_control_mode(uint8_t);
    void exit_control_mode(uint8_t);
    void set_position_zero(uint8_t);
};
#endif  // CAN_com
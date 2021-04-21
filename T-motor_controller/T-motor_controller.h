#ifndef T_MOTOR_CONTROLLER_H_
#define T_MOTOR_CONTROLLER_H_

#include <vector>
#include "can_com.h"
#include "motor_status.h"
#include "basic_op.h"

class T_motor_controller{
private:
    const PinName can_rx_pin = PB_8;
    const PinName can_tx_pin = PB_9;

    CAN_com can_com;
    std::vector<Motor_Status> motor;

public:
    // Constructors
    T_motor_controller(void);
    T_motor_controller(const PinName rx, const PinName tx);

    virtual void control(void);
};
#endif  // T-motor_controller
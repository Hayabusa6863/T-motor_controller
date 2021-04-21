#include "T-motor_controller.h"

T_motor_controller::T_motor_controller(void)
    : can_com(can_rx_pin, can_tx_pin)
{

}

T_motor_controller::T_motor_controller(const PinName rx, const PinName tx)
    : can_com(rx, tx)
{

}
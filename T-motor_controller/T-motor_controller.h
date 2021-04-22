#ifndef T_MOTOR_CONTROLLER_H_
#define T_MOTOR_CONTROLLER_H_

#include <vector>
#include "can_com.h"
#include "motor_status.h"
#include "basic_op.h"

class T_motor_controller{
private:
    DigitalOut led;
    CAN can;    // canのアドレスを渡す形にするべきか，can_com.cppにcanの実態を置くべきか？
protected:
    std::vector<Motor_Status> motor;
    // virtual void control(void) = 0; // トルク指令を計算

public:
    // Constructors
    T_motor_controller(void);
    ~T_motor_controller();

};
#endif  // T-motor_controller
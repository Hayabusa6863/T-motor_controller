#ifndef T_MOTOR_CONTROLLER_H_
#define T_MOTOR_CONTROLLER_H_

#include <cstdint>
#include <utility>
#include <map>
#include <vector>
#include "can_com.h"
#include "motor_status.h"
#include "basic_op.h"

// 動かしたいモータの配列
const uint8_t motor_to_control[] = {CAN_MOTOR_ID};

class T_motor_controller{
private:
    const PinName can_rx_pin = PB_8;
    const PinName can_tx_pin = PB_9;
    const int can_baudrate = 1000000;

    DigitalOut led;
    CAN_com can_com;
    CAN can;
    
    // モータIDとモータ情報クラスのペア
    std::vector<Motor_Status> motor;
    std::map<uint8_t, Motor_Status*> motor_id;

    void addMotor(uint8_t); // モータの追加
    void can_callback(void);

protected:
    // virtual void control(void) = 0; // トルク指令を計算
    void startControl(void);

public:
    // Constructors
    T_motor_controller(void);
    ~T_motor_controller();

    void enableController(void);
    void disableController(void);

};
#endif  // T-motor_controller
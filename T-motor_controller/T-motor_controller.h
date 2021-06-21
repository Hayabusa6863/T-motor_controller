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
    
    Thread thread;  // 送信用のスレッド

    DigitalOut led;
    CAN_com can_com;
    CAN can;
    
    std::map<uint8_t, Motor_Status*> motor_id;  // モータのCAN_IDとモータ情報クラスポインタのペア

    void add_motor(void);       // モータの呼び出し
    void can_callback(void);    // CAN受信割込み関数
    void can_send_thread(void);        // CAN送信スレッド

protected:
    virtual void control(void) = 0;     // トルク指令を計算
    std::vector<Motor_Status> motor;    
    void enableController(void);        // コントローラを有効化
    void disableController(void);       // コントローラを無効化
    
public:
    // Constructors
    T_motor_controller(void);
    ~T_motor_controller();

};
#endif  // T-motor_controller
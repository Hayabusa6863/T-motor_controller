#ifndef MOTOR_STATUS_H_
#define MOTOR_STATUS_H_

#include "mbed.h"

class Motor_Status{
private:
    uint8_t can_id; // モータのCAN_ID

    float position; // 現在位置
    float velocity; // 現在速度
    float effort;   // 現在トルク

    float effort_ref;   // 目標トルク

public:
    Motor_Status(uint8_t can_id_);
    
    void setPosition(float pos);
    void setVelocity(float vel);
    void setEffort(float eff);
    void setTargetEffort(float tau_ref_);   // 目標トルクのセット

    float getPosition(void);
    float getVelocity(void);
    float getEffort(void);
};

#endif  // Motor_Status
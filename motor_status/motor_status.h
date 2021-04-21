#ifndef MOTOR_STATUS_H_
#define MOTOR_STATUS_H_

#define P_MAX   12.5f
#define P_MIN   -12.5f
#define V_MAX   45.0f
#define V_MIN   -45.0f
#define T_MAX   18.0f
#define T_MIN   -18.0f

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
    void setTargetEffort(float tau_ref_);   // 目標トルクのセット
    float getPosition(void);
    float getVelocity(void);
    float getEffort(void);
};

#endif  // Motor_Status
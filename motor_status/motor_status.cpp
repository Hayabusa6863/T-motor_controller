#include "motor_status.h"

Motor_Status::Motor_Status(uint8_t can_id_)
    : can_id(can_id_), 
      position(0.0f),
      velocity(0.0f),
      effort(0.0f),
      effort_ref(0.0f){}

void Motor_Status::setTargetEffort(float tau_ref_){
    effort_ref = tau_ref_;
}

float Motor_Status::getPosition(void){
    return position;
}

float Motor_Status::getVelocity(void){
    return velocity;
}

float Motor_Status::getEffort(void){
    return effort;
}
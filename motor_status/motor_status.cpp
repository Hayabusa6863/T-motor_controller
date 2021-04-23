#include "motor_status.h"

Motor_Status::Motor_Status(uint8_t can_id_)
    : can_id(can_id_),
      position(0.0f),
      velocity(0.0f),
      effort(0.0f),
      effort_ref(0.0f){}

void Motor_Status::setPosition(float pos){ position = pos; }
void Motor_Status::setVelocity(float vel){ velocity = vel; }
void Motor_Status::setEffort(float eff){ effort = eff; }
void Motor_Status::setTargetEffort(float tau_ref_){ effort_ref = tau_ref_; }

const float Motor_Status::getPosition(void){ return position; }
const float Motor_Status::getVelocity(void){ return velocity; }
const float Motor_Status::getEffort(void){ return effort; }
const float Motor_Status::getTargetEffort(void){ return effort_ref; }
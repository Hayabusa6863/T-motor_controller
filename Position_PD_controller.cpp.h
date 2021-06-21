// Position PD controller
// トルク制御による位置PD制御

#include "T-motor_controller.h"

class Position_PD_controller : public T_motor_controller{
private:
    const float Kp = 1.0;
    const float Kd = 0.5;

    // トルクに関する制御式
    virtual void control(void) override{
        float tau_ref = 0;
        motor[0].setTargetEffort(tau_ref);
    }

public:
    // コンストラクタ．スレッドの開始
    Position_PD_controller(void){
        
    };   
};


int main(void){
    T_motor_controller *controller = new Position_PD_controller;


}
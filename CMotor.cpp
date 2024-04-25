#include "CMotor.h"

CMotor::CMotor() {

}

CMotor::~CMotor() {
    // Destructor implementation
}

void CMotor::set_pwm_left(int val) {
    // Set PWM for left motor
}

void CMotor::set_pwm_right(int val) {
    // Set PWM for right motor
}

int CMotor::get_pwm_left() {
    // Return left PWM value
    return 0;
}

int CMotor::get_pwm_right() {
    // Return right PWM value
    return 0;
}

void CMotor::stop() {
    // Stop motors
}

void CMotor::forwards(int time) {

    /*
    //enable,reset,sleep,step,direction
    //outputPinVector = {5,6,13,19,26};
    */
    // Move forward
    //_control.get_button(_control.GPIO_PIN21_INPUT);
    //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
    auto finishTime = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
    while(finishTime >  std::chrono::system_clock::now()){
        int result = 1;
        //_control.lightsAndButtons(_control.GPIO_PIN2_INPUT,_control.GPIO_PIN3_OUTPUT, result);
        _control.set_data(PWM,_pinleftPWM,255);// we can multithread this in the future.
        _control.set_data(PWM,_pinrightPWM,255);// forgot to test speed at 255

       // _control.set_data(PWM,6,200);
        //_control.set_data(DIGITAL,5,1);//0
    }
}

void CMotor::backward(int time) {
    // Move backward
    //_control.set_data(PWM,13,255);// we can multithread this in the future.
    //_control.set_data(PWM,12,255);// forgot to test speed at 255

}

void CMotor::left(int time) {
    int timeMS = 1000;
    //for a true 90 degree turn, we switch direction of right
    _control.set_data(PWM,_pinleftPWM,255);// we can multithread this in the future.
    _control.set_data(PWM,_pinrightPWM,0);// forgot to test speed at 255

}

void CMotor::right(int time) {
    int timeMS = 1000;
    _control.set_data(PWM,_pinleftPWM,0);// we can multithread this in the future.
    _control.set_data(PWM,_pinrightPWM,255);// forgot to test speed at 255
}

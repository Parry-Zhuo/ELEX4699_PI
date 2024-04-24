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
    // Move forward
    //_control.get_button(_control.GPIO_PIN21_INPUT);
    //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
    auto finishTime = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
    while(finishTime >  std::chrono::system_clock::now()){
        int result = 1;
        //_control.lightsAndButtons(_control.GPIO_PIN2_INPUT,_control.GPIO_PIN3_OUTPUT, result);
        _control.set_data(PWM,12,10);
    }
}

void CMotor::backward(int time) {
    // Move backward
}

void CMotor::left(int time) {
    // Turn left
}

void CMotor::right(int time) {
    // Turn right
}

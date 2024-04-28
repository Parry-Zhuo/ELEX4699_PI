#include "CMotor.h"


#define _pinleftPWM 12
#define _pinrightPWM 13

#define _pinleftDIR 6
#define _pinrightDIR 16

#define _pinEnableMotors 26
CMotor::CMotor() {
    _leftPWM = 0;
    _rightPWM = 0;

    _leftDIR = 0;
    _rightDIR = 0;

    //_accelerationTable = {500,500,500,500,1000,1000,1500,1500,2000,2000};
    _lastCommand = CRY;
    _consecutiveCount = 0;

    _control.set_data(DIGITAL,_control._pinEnLmotor,1);
    _control.set_data(DIGITAL,_control._pinEnRmotor,1);
}

CMotor::~CMotor() {
    // Destructor implementation
    _control.set_data(DIGITAL,_control._pinEnLmotor,1);
    _control.set_data(DIGITAL,_control._pinEnRmotor,1);
}
void CMotor::enableMotor() {
    _control.set_data(DIGITAL,_control._pinEnLmotor,0);
    _control.set_data(DIGITAL,_control._pinEnRmotor,0);
}
void CMotor::setLeftDir(int dir) {
    _leftDIR = dir;
    _control.set_data(DIGITAL,_pinleftDIR,_leftDIR);
}
void CMotor::setRightDir(int dir) {
    _rightDIR =dir;
    _control.set_data(DIGITAL,_pinrightDIR,_rightDIR);
}
void CMotor::set_pwm_left(int val) {
    // Set PWM for left motor
    _leftPWM = val;
}
void CMotor::set_pwm_right(int val) {
    // Set PWM for right motor
    _rightPWM = val;
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
int lagAccomodation = 0;
void CMotor::forwards(int time) {

    /*
    //enable,reset,sleep,step,direction
    //outputPinVector = {5,6,13,19,26};
    */
    // Move forward
    //_control.get_button(_control.GPIO_PIN21_INPUT);
    //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
    //auto finishTime = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
   // while(finishTime >  std::chrono::system_clock::now()){
     //   int result = 1;
        //_control.lightsAndButtons(_control.GPIO_PIN2_INPUT,_control.GPIO_PIN3_OUTPUT, result);
    //int maxFreq = 1500;
    /*
    if(_lastCommand == FORWARD){
        _leftPWM += 100;
        _rightPWM +=100;
        if(_leftPWM >= maxFreq){
            _leftPWM = maxFreq;
            _rightPWM = maxFreq;
        }
    }else if(lagAccomodation >=3){
        _leftPWM =800;
        _rightPWM =800;
        lagAccomodation = 0;
    }else{
        lagAccomodation++;
    }
    */
    _leftPWM = 8000;
    _rightPWM = 8000;
    gpioSetPWMfrequency(_pinleftPWM, _leftPWM);
    gpioSetPWMfrequency(_pinrightPWM, _rightPWM);

    std::cout<<"_rightPWM: " << _rightPWM << "\n";
    _control.set_data(PWM,_pinleftPWM,126);// we can multithread this in the future.
    _control.set_data(PWM,_pinrightPWM,126);
       // _control.set_data(PWM,6,200);
        //_control.set_data(DIGITAL,5,1);//0
    //}
    _lastCommand = FORWARD;
}

void CMotor::backward(int time) {
    // Move backward

    setRightDir(0);
    setLeftDir(0);

    _leftPWM = 8000;
    _rightPWM = 8000;
    gpioSetPWMfrequency(_pinleftPWM, _leftPWM);
    gpioSetPWMfrequency(_pinrightPWM, _rightPWM);

    _control.set_data(PWM,13,126);// we can multithread this in the future.
    _control.set_data(PWM,12,126);// forgot to test speed at 255

    //setRightDir(0);
    //setLeftDir(0);
    _lastCommand = BACKWARD;
}
//turns right
void CMotor::left(int time) {
    //int timeMS = 1000;
    //for a true 90 degree turn, we switch direction of right
    _leftPWM = 8000;
    _rightPWM = 0;

    gpioSetPWMfrequency(_pinleftPWM, _leftPWM);
    gpioSetPWMfrequency(_pinrightPWM, _rightPWM);
    _control.set_data(PWM,_pinleftPWM,126);// we can multithread this in the future.
    _control.set_data(PWM,_pinrightPWM,0);// forgot to test speed at 255
    _lastCommand = LEFT;
}
//turns left
void CMotor::right(int time) {
    //int timeMS = 1000;
    _leftPWM = 0;
    _rightPWM = 8000;
    gpioSetPWMfrequency(_pinleftPWM, _leftPWM);
    gpioSetPWMfrequency(_pinrightPWM, _rightPWM);
    _control.set_data(PWM,_pinleftPWM,0);// we can multithread this in the future.
    _control.set_data(PWM,_pinrightPWM,126);// forgot to test speed at 255

    _lastCommand = RIGHT;
}
void CMotor::cry(int time) {
    int i = 0;
    _leftPWM = 0;
    _rightPWM = 0;
    setRightDir(1);
    setLeftDir(1);
    //disable MOTOR??? Probably not.
    _control.set_data(PWM,_pinleftPWM,0);// we can multithread this in the future.
    _control.set_data(PWM,_pinrightPWM,0);
    _lastCommand = CRY;
}

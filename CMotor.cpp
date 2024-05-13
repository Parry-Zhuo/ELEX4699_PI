#include "CMotor.h"


CMotor::CMotor() {

    _lastCommand = ' ';
    _consecutiveCount = 0;
    //ENSURE MOTOR DRIVER IS TURNED OFF TO CONSERVE POWER
    _control.set_data(DIGITAL,_control._pinEnLmotor,1);
    _control.set_data(DIGITAL,_control._pinEnRmotor,1);

    _leftPWM = 4000;
    _rightPWM = 4000;


    gpioSetPWMfrequency(_control._pinleftPWM, _leftPWM);
    gpioSetPWMfrequency(_control._pinrightPWM, _rightPWM);
    maxFreq = 6000;
}

CMotor::~CMotor() {
    // Destructor implementation
    _control.set_data(DIGITAL,_control._pinEnLmotor,1);
    _control.set_data(DIGITAL,_control._pinEnRmotor,1);

}
void CMotor::enableMotor(int state ) {//default state is 0
    _control.set_data(DIGITAL,_control._pinEnLmotor,state);
    _control.set_data(DIGITAL,_control._pinEnRmotor,state);
}
void CMotor::setLeftDir(int dir) {
    _leftDIR = dir;
    _control.set_data(DIGITAL,_control._pinleftDIR,_leftDIR);
}
void CMotor::setRightDir(int dir) {
    _rightDIR =dir;
    _control.set_data(DIGITAL,_control._pinrightDIR,_rightDIR);
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
    _leftPWM = 0;
    _rightPWM = 0;
    setRightDir(1);
    setLeftDir(1);

    _control.set_data(PWM,_control._pinleftPWM,0);// we can multithread this in the future.
    _control.set_data(PWM,_control._pinrightPWM,0);// forgot to test speed at 255
    _lastCommand = ' ';
}

void CMotor::shoot(int position){
//    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(50));

    if(position == SHOOTPOS){
       // _ball1Count++;
       std::cout<< "shoot! \n";
       //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
        //_control.servoControl(_control._pinShootPWM, 150);
        //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(800));
        _control.servoControl(_control._pinShootPWM, 30);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(300));
        _control.servoControl(_control._pinShootPWM, 150);
    }else if(position == RELOADPOS){

    }
}

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
    if(time == 0){
        _control.set_data(PWM,_control._pinleftPWM,126);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,126);
    }
    /*
    if(_lastCommand == 'w'){

        std::cout << "freq" << _leftPWM << "\n";
        if(_leftPWM >= maxFreq){

            _leftPWM = maxFreq;
            _rightPWM = maxFreq;
        }else if(_leftPWM % 500 == 0){
            gpioSetPWMfrequency(_control._pinleftPWM, _leftPWM);
            gpioSetPWMfrequency(_control._pinrightPWM, _rightPWM);
            _leftPWM += 5;
            _rightPWM +=5;
        }else{
            _leftPWM += 5;
            _rightPWM +=5;

        }
    }else{
        setRightDir(1);
        setLeftDir(1);
        _leftPWM = 4000;
        _rightPWM = 4000;
        //forwards();
    }
    */

    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
    auto finishTime = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
    do {

       // std::cout<<"_rightPWM: " << _rightPWM << "\n";
        _control.set_data(PWM,_control._pinleftPWM,126);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,126);
    }while(finishTime >  std::chrono::system_clock::now());
    _lastCommand = 'w';

}

void CMotor::backward(int time) {
    // Move backward

    setRightDir(0);
    setLeftDir(0);
    _lastCommand = 's';
        if(time == 0){
        _control.set_data(PWM,_control._pinleftPWM,126);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,126);
        return;
    }
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
    auto finishTime = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
    do {
        //std::cout<<"_rightPWM: " << _rightPWM << "\n";
        _control.set_data(PWM,_control._pinleftPWM,126);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,126);
    }while(finishTime >  std::chrono::system_clock::now());

}
//turns right
void CMotor::left(int time) {

    _lastCommand = 'd';
    if(time == 0){
        _control.set_data(PWM,_control._pinleftPWM,126);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,0);
        return;
    }
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
    auto finishTime = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
    do {
       // std::cout<<"_rightPWM: " << _rightPWM << "\n";
        _control.set_data(PWM,_control._pinleftPWM,126);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,0);
    }while(finishTime >  std::chrono::system_clock::now());

}
//turns left
void CMotor::right(int time) {

    _lastCommand = 'a';
    if(time == 0){
        _control.set_data(PWM,_control._pinleftPWM,0);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,126);
        return;
    }
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
    auto finishTime = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
    do{

        std::cout<<"_rightPWM: " << _rightPWM << "\n";
        _control.set_data(PWM,_control._pinleftPWM,0);// we can multithread this in the future.
        _control.set_data(PWM,_control._pinrightPWM,126);
    }while(finishTime >  std::chrono::system_clock::now());

}
void CMotor::cry(int time) {
    int i = 0;
    _leftPWM = 0;
    _rightPWM = 0;
    setRightDir(1);
    setLeftDir(1);
    //disable MOTOR??? Probably not.
    _control.set_data(PWM,_control._pinleftPWM,0);// we can multithread this in the future.
    _control.set_data(PWM,_control._pinrightPWM,0);
    _lastCommand = 'x';
}

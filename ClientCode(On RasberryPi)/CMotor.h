/**
 * @brief Controls the H-bridge to drive the system forwards, left, right and backwards. Various speeds can
be set as well as incremental angles of rotation for turning
 * Detailed explanation of the class, its role, responsibilities, and behavior.
 PSEUDO CODE

 So we have 2 pins. which drive the motors. 1.2A for maximum load(GPT)

 1. ALLOCATE 2 GPIO PINS FOR DIRECTION- denoted _pinA and _pinB
 2. ALLOCATE 1 PIN FOR PWM - FOR SPEED - attach to enable pin
 */
#pragma once

#include "CControler.h"
#include <thread>
class CMotor
{
private:
    enum typeEnum { DIGITAL = 0, ANALOG, SERVO,PWM };//need to add into get_Data
    //enum statesEnum { FORWARD = 0, BACKWARD, LEFT, RIGHT, STOP,CRY,SHOOT};
    int maxFreq= 6000;
    enum cameraEnum { LEFT= -1, STARE = 0, RIGHT = 1};
public:
    enum typeShootPosEnum {RELOADPOS = 0, SHOOTPOS };
    /*Initialization of variables.*/


    int _leftDIR,_rightDIR;///direction of the motor 1 is back, 0 is forward probably
    int _leftPWM,_rightPWM;///refers to the value of PWM on that MOTOR


	CMotor();
	~CMotor();
	void setLeftDir(int val);
	void setRightDir(int val);

	void set_pwm_left(int val);
	void set_pwm_right(int val);
	int get_pwm_left();
	int get_pwm_right();
	void stop();
	void forwards(int time=  0);
	void backward(int time = 0);
	void left(int time = 0);
	void right(int time = 0);
	void cry(int time = 0);

    void enableMotor(int state = 0);//standard state is on.
    void shoot(int position = 1);
    void look(int direction = 0);
	void changeFreq(int newFreq);
	CControl _control;

    char _lastCommand;
    int _consecutiveCount;
    int cameraState = 0;
};

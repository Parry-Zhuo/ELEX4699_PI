/**
 * @file CControler.cpp
 * @brief Implementation file for the Calculator class.
 *
 * Implements the methods of the Calculator class defined in Example.h.
 *
 */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "CControler.h"
#include "pigpio.h"
#include <opencv2/opencv.hpp>

//#include "Serial.h"
#define _pinleftPWM 12
#define _pinrightPWM 13

#define _pinleftDIR 6
#define _pinrightDIR 16

CControl::CControl() {

    if (gpioInitialise() < 0)
    {
        std::cout << "GPIO INITILIZATION FAILUREn\n\n\n";
    }else{
        std::cout << "GPIO SUCCESS Inside CCONTROL CLASS \n\n\n";


        gpioSetMode(GPIO_PIN3_OUTPUT, PI_OUTPUT); // Setup PINOUT1 as Output
        gpioSetMode(GPIO_PIN4_ANALOG, PI_OUTPUT);

        //GPIO 12 AND 13 ARE PWM PINS!
        gpioSetMode(_pinleftPWM, PI_ALT0);//LEFT CHANNEL
        gpioSetMode(_pinrightPWM, PI_ALT0);//RIGHT CHANNEL
        gpioSetPullUpDown(_pinleftPWM, PI_PUD_UP);
        gpioSetPullUpDown(_pinrightPWM, PI_PUD_UP);
        //gpioSetMode(12, PI_INPUT);
        //gpioSetPWMfrequency(_pinleftPWM, 25000);
        gpioSetPWMfrequency(_pinleftPWM, 500);
        gpioSetPWMfrequency(_pinrightPWM, 500);
        //enable,reset,sleep,step,direction
        outputPinVector = {1,5,6,7,19,26,13,2,3};
        for(int x = 0;x< outputPinVector.size();x++){//set inputs
            gpioSetMode(outputPinVector.at(x), PI_OUTPUT);
        }

        inputPinVector = {2,16,20,21};
        for(int x = 0;x< inputPinVector.size();x++){//set inputs
            gpioSetMode(inputPinVector.at(x), PI_INPUT);
        }
        for(int x = 0;x< inputPinVector.size();x++){//set inputs
            gpioSetPullUpDown(inputPinVector.at(x), PI_PUD_UP);
        }
        buttonVector = std::vector<buttonStruct>(30);
        gpioWrite(GPIO_PIN3_OUTPUT, 0);
        handle = spiOpen(0, 200000, 3); // Mode 0, 200kHz
    }
}
CControl::~CControl() {
	std::cout << "Controller has been destryoed. Destructor has been called. n\n\n\n";
    gpioTerminate();
    spiClose(handle); // Close SPI system

}

int CControl::init_com(int comport) {

}
/**
	 * @brief verifies communication is working
	 * @param comNum com number used
	 * @param bitrate - baudrate thingy
	 * @return The function will return false if it wasn't able to detect the board.
	 */

bool CControl::verifyCom(int comNum, int bitRate) {
/*
	int test = 0;
	bool isVerified = (set_data(3, 0, test) == false);
	_com.flush();
	return isVerified;
	*/
}

/*This function given type(Analog,digital,servo) and channel will

The getData will send data as the form
"g type channel"
Expecting return of
"a type channel result"


return the value measured on TM4C123 MCU onto servo.

The function will return true, if it was able to successfully measure the desired input

*/
bool CControl::get_data(int type, int channel, int& result) {
    if(type == DIGITAL){
        result = gpioRead(channel);
               //std::cout<< "resuly " << result << "\n";
        if(result < 0 ){//issue with reading data.
            return false;
        }
        return true;
    }else if(type == ANALOG){
        // erm.
    }

}
//get_analog calls data function
int CControl::get_analog(int channel) {
    int read_val;
    unsigned char inBuf[3];
    char cmd[] = { 1, 128, 0 }; // 0b1XXX0000 where XXX=channel 0
    spiXfer(handle, cmd, (char*) inBuf, 3); // Transfer 3 bytes
    read_val = ((inBuf[1] & 3) << 8) | inBuf[2]; // Format 10 bits
    std::cout<<"analog value: " << read_val << " inbuf: "<<inBuf<<"\n";
	return read_val*100/1024;
}

bool CControl::set_data(int type, int channel, int val) {
    if(type == DIGITAL){//if digital you may write
        gpioWrite(channel,val);
    }else if(type ==SERVO){
        std::cout<< "Servo called! " << val << "\n";
        gpioServo(channel,val);
    }else if(type == PWM){
        if(gpioPWM(channel, val) == 0){
            //successful
            std::cout<< "PWM SIGNAL WORKED" << val << "channel: "<< channel<<  "\n";
        }else{
            std::cout<< "UNSUCCESSUL PWM SIGNAL" << val << "channel: "<< channel << "\n";
        }
    }else{
        return false;
    }
    return true;
}

int CControl::get_button(int channel) {
    int result;
    get_data(DIGITAL,channel, result);
    buttonStruct currentButton = buttonVector.at(channel);//button struct was not properly defined...
    std::cout << "RESULT: " << result  << "\n";
    if(result != currentButton.lastState){
        buttonVector.at(channel).lastDebounceTime = float(cv::getTickCount()) / cv::getTickFrequency() * 1000;
    }
	if ((float(cv::getTickCount()) / cv::getTickFrequency() * 1000) - currentButton.lastDebounceTime > debounceDelay) {
		if (result != currentButton.lastState) {// we may want to just simply remove this.
			buttonVector.at(channel).buttonState = result;
			if (buttonVector.at(channel).buttonState == ledON) {
				//button has been stabilized
				buttonVector.at(channel).output ^= 1;
				buttonVector.at(channel).buttonCount++;

			}
		}
	}
	buttonVector.at(channel).lastState = result;
	return buttonVector.at(channel).output;
}

int CControl::get_button2(int channel) {
    //std::cout<<"button 2: "<< "\n";
	static int result;
	static int output;//determines the state
	//float start_time = 0;
	static float lastDebounceTime = 0;
	static int buttonState; // Current reading from the button
	static int lastButtonState = ledOFF; // Previous state of the button
	static int debounceDelay = 10;
	int timeout = 1000;
	static int count = 0;
	get_data(DIGITAL, channel, result); // initial detection of button press
	if (result != lastButtonState) {
		lastDebounceTime = float(cv::getTickCount()) / cv::getTickFrequency() * 1000;
	}
	if ((float(cv::getTickCount()) / cv::getTickFrequency() * 1000) - lastDebounceTime > debounceDelay) {
		if (result != buttonState) {
			buttonState = result;
			if (buttonState == ledON) {
				//button has been stabilized
				output ^= 1;
				count++;
				std::cout << "BUTTON TEST : " << count << "\n";
			}
		}
	}
	lastButtonState = result;
	return output;

}
int CControl::get_button3(int channel) {

	static int result;
	static int output;//determines the state
	//float start_time = 0;
	static float lastDebounceTime = 0;
	static int buttonState; // Current reading from the button
	static int lastButtonState = ledOFF; // Previous state of the button
	static int debounceDelay = 10;
	int timeout = 1000;
	static int count = 0;
	get_data(DIGITAL, channel, result); // initial detection of button press
	if (result != lastButtonState) {
		lastDebounceTime = float(cv::getTickCount()) / cv::getTickFrequency() * 1000;
	}
	if ((float(cv::getTickCount()) / cv::getTickFrequency() * 1000) - lastDebounceTime > debounceDelay) {
		if (result != buttonState) {
			buttonState = result;
			if (buttonState == ledON) {
				//button has been stabilized
				output = 1;
				count++;
				std::cout << "BUTTON TEST : " << count << "\n";
			}
		}
	}
	lastButtonState = result;
	return output;
}

/*
This class correlates an digital input(BUTTONS), with an digital output(LED)3
*/
void CControl::lightsAndButtons(int inputChannel, int outputChannel, int &result) {
    if(get_data(DIGITAL, inputChannel,result) == false){
        std::cout<< "Unable to get data" << "\n";
        return;
    }
    if(result== 1){ // 1 implies on
        set_data(DIGITAL, outputChannel,1);
    }else{
        set_data(DIGITAL, outputChannel,0);
    }
}

/*Takes in 2 inputs, outputing
*/
void CControl::analogInputs(int channel1, int channel2, cv::Point &xyJoystick) {
	int analogValue1= get_analog(channel1);
	int analogValue2 = get_analog(channel2);
	xyJoystick = cv::Point(analogValue1, analogValue2);
}

void CControl::servoControl(int inputChannel, int angle) {
	//set_data(SERVO, inputChannel, angle);
	/*
	0 -> 500
	180 -> 2500

	angle =((2000)/180)angle + 500

	test, if angle = 180 --> 2500

                //int angle = 500;
                //int maxAngle = 2400;

	*/
	angle =((2400-500)/180) * angle + 500;

	gpioServo(inputChannel,angle);
}

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

CControl::CControl() {

    if (gpioInitialise() < 0)
    {
        std::cout << "GPIO INITILIZATION FAILUREn\n\n\n";
    }else{
        std::cout << "GPIO SUCCESS Inside CCONTROL CLASS \n\n\n";

        GPIO_PIN2_INPUT = 2;
        GPIO_PIN3_OUTPUT = 3;
        GPIO_PIN4_ANALOG = 17;

        GPIO_PIN16_INPUT = 16;
        GPIO_PIN20_INPUT = 20;
        GPIO_PIN21_INPUT = 21;

        gpioSetMode(GPIO_PIN3_OUTPUT, PI_OUTPUT); // Setup PINOUT1 as Output
        gpioSetMode(GPIO_PIN2_INPUT, PI_INPUT); // Setup PININ1 as Input

        gpioSetMode(GPIO_PIN16_INPUT, PI_INPUT);
        gpioSetMode(GPIO_PIN20_INPUT, PI_INPUT);
        gpioSetMode(GPIO_PIN21_INPUT, PI_INPUT);

        gpioSetMode(GPIO_PIN4_ANALOG, PI_OUTPUT);
        /*
        gpioWrite(PINOUT1, 0);
            */
        gpioWrite(GPIO_PIN3_OUTPUT, 0);


        gpioSetPullUpDown(GPIO_PIN2_INPUT, PI_PUD_DOWN);

        gpioSetPullUpDown(GPIO_PIN16_INPUT, PI_PUD_DOWN);
        gpioSetPullUpDown(GPIO_PIN20_INPUT, PI_PUD_DOWN);
        gpioSetPullUpDown(GPIO_PIN21_INPUT, PI_PUD_DOWN);

//        int in = gpioRead(GPIO_PIN2_INPUT); // 1 implies on

        /*
        Initialize SPI
        */

        handle = spiOpen(0, 200000, 3); // Mode 0, 200kHz

        //spiClose(handle); // Close SPI system



        /*
        int read_val;
        unsigned char inBuf[3];
        char cmd[] = { 1, 128, 0 }; // 0b1XXX0000 where XXX=channel 0
        handle = spiOpen(1, 200000, 3);
        if (handle < 0)
        {
            std::cout << "Failed to open SPI connection" << std::endl;
        }
        else{
            std::cout << "OPENED" << std::endl;
        }
        spiXfer(handle, cmd, (char*) inBuf, 3); // Transfer 3 bytes
        read_val = ((inBuf[1] & 3) << 8) | inBuf[2]; // Format 10 bits

        std::cout<<"analog value: " << read_val << "\n";
        */




      //  std::cout<<in << "first: \n";
        //gpioTerminate();
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


enum typeEnum { DIGITAL = 0, ANALOG, SERVO };//need to add into get_Data
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
    }else{
        return false;
    }
    return true;
}

enum ledStateENUM { ledON = 0, ledOFF };//need to add into get_Data
int CControl::get_button(int channel) {
	static int result;
	static int output;//determines the state
	//float start_time = 0;
	static float lastDebounceTime = 0;
	static int buttonState; // Current readin
	//g from the button
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

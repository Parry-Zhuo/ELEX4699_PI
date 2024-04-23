/**
 * @file CControler.h
 * @brief Header file for the CControl class.
 *
 * This file provides the declarations for the CControl class, which is designed for managing serial communication
 * and controlling connected devices. The class offers capabilities to initialize serial communication, perform
 * read and write operations on various data types and channels, and manage devices such as servos and buttons
 * through a serial interface.
 * @author Parry Zhuo
 */
/*
#pragma once
#include "stdafx.h"
#include "Serial.h"
#include <vector>


 /**
  * @class CControl
  * @brief Manages serial communication and control of connected devices. Using NMEA 0183 Protocol
  *
  * CControl facilitates serial communication with hardware components, enabling initialization,
  * data reading/writing, and device control. It supports various operations including reading analog
  * and digital inputs, controlling servos, and managing button states with debouncing.
  */
#pragma once
#include "pigpio.h"
#include <opencv2/opencv.hpp>
class CControl {
private:
//    Serial _com; ///< Serial communication port object for interfacing with hardware.

    enum typeEnum { DIGITAL = 0, ANALOG, SERVO };//need to add into get_Data


    int button1Count = 0;
    int button2Count= 0;
    int button3Count= 0;

    bool outputButton1 = 0;
    bool outputButton2 = 0;
    bool outputButton3 = 0;
    /*To be destroyed later*/

    /*To be destroyed later*/

    bool threadExit = true;


public:
    /**
     * @brief Constructs the CControl object and initializes serial communication.
     */
    CControl();

    /**
     * @brief Destructs the CControl object, ensuring proper cleanup of serial resources.
     */
    ~CControl();

    /**
     * @brief Initializes the serial communication port with a specified COM port number.
     * @param comport The COM port number to use for communication.
     */
    int init_com(int comport);
    /**
     * @brief verify that the COM port is still working.
     * @param returns true if it is working, false if comPort is not working.
     */
    bool verifyCom(int comNum, int bitRate);
    /**
     * @brief Reads data from a specified channel and type. Sends command ( g type channel )
     * @param type The type of data to read (e.g., DIGITAL, ANALOG, SERVO).
     * @param channel The channel number to read from.
     * @param result Reference to store the read data.
     * @return True if command was returned from MCU, false if returned value was incorrect or non-existant.
     */
    bool get_data(int type, int channel, int& result);

    /**
     * @brief The getData will send data as the form
        "g type channel"
        Expecting return of
        "a type channel result"
     *
     *
     *
     * @param type The type of data to set.
     * @param channel The channel to which to set the data.
     * @param val The value to set.
     * @return The function will return true, if it was able to successfully measure the desired input
     */
    bool set_data(int type, int channel, int val);

    /**
     * @brief Reads analog data from a specified channel usually potentiometer. 12 bit ADC
     * @param channel The analog channel to read from.
     * @return percentage analog value read
     */
    int get_analog(int channel);

    /**
     * @brief Reads the state of a button from a specified channel, with debouncing.
     * @param channel The button channel to read from.
     * @return 1 if pressed, 0 if not pressed
     */
    int get_button(int channel);
    int get_button2(int channel);
    int get_button3(int channel);
    /**
     * @brief Controls lights and reads button states using specified channels. Using level detection button.
     * @param inputChannel The channel to read the button state from.
     * @param outputChannel The channel to control the light.
     * @param result Reference to store the button state read.
     */
    void lightsAndButtons(int inputChannel, int outputChannel, int& result);

    /**
     * @brief Reads analog inputs from two channels, returning their values in a percentage point.
     * to return analog value xyJoystick.x * 4096 / 100
     * @param channel1 The first analog channel to read from.
     * @param channel2 The second analog channel to read from.
     * @param xyJoystick, returns by reference the xy PERCENTAGE values of joystick
     */
    void analogInputs(int channel1 , int channel2 , cv::Point& xyJoystick);
    /**
     * @brief Reads analog inputs from two channels, returning their values. Overloaded verseion. For XY joystick
     * to return analog value xyJoystick.x * 4096 / 100
     * @param xyJoystick, returns by reference the xy PERCENTAGE values of joystick
     */

    void analogInputs(cv::Point& xyJoystick) {
        int xChannel = 11; // Default value for channel1
        int yChannel = 4; // Default value for channel2
        analogInputs(xChannel, yChannel, xyJoystick);
    }
    /**
     * @brief Controls a servo motor's position by setting its angle.
     * @param inputChannel The channel the servo is connected to.
     * @param angle The desired angle to set the servo to.
     */
    void servoControl(int inputChannel, int angle);

    int GPIO_PIN5_ANALOG;

    int GPIO_PIN3_OUTPUT = 3;
    int GPIO_PIN4_ANALOG = 17;

    int GPIO_PIN2_INPUT = 2;
    int GPIO_PIN16_INPUT = 16;
    int GPIO_PIN20_INPUT = 20;
    int GPIO_PIN21_INPUT = 21;

    int GPIO_PWM1 = 13;
    int GPIO_PWM2 = 12;

    int debounceDelay = 10;
    enum ledStateENUM{ ledON = 0, ledOFF };//need to add into get_Data
    int handle ;

    ;//extern  tells the compiler that the array is defined elsewhere, avoiding multiple definitions and linkage errors.
    struct buttonStruct{
        //int channel = 0;//not needed as it is the index
        float lastDebounceTime = 0;
        int lastState = ledOFF;
        int buttonCount = 0;//for now it is nothing, just incase.
        int output = ledOFF;
        int buttonState = ledOFF;// may need to create new definition variable!
    };
    std::vector<buttonStruct> buttonVector;
    std::vector<int> inputPinVector;

};


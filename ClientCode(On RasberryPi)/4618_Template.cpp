////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated Dec 2, 2022
////////////////////////////////////////////////////////////////
//#include "stdafx.h"
//#include <opencv2/core.hpp>

// Add simple GUI elements
#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <thread>

#include "pigpio.h"
#include <opencv2/opencv.hpp>
//#include "CControler.h"
//#include "PPong.h"
//#include "CRecyclingSort.h"
#include "CCar.h"

#include <opencv2/aruco.hpp>



#include <thread>

#include "Client.h"
#include "server.h"

// Must include Windows.h after Winsock2.h, so Serial must be included after Client/Server
//#include "Serial.h"


//#include "CSketch.h"
//#include "CBase4618.h"

//#include "PPong.h"


#define CANVAS_NAME "Display Image"
////////////////////////////////////////////////////////////////
// Can be used as a replacement for cv::waitKey() to display cv::imshow() images, Windows Only
////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////
// Lab 2
////////////////////////////////////////////////////////////////
void lab1() {
    // Create a window
    cv::namedWindow("Example", cv::WINDOW_AUTOSIZE);

    // Display an empty window
    cv::Mat emptyImage = cv::Mat::zeros(480, 640, CV_8UC3);  // Creates a black window
    cv::imshow("Example", emptyImage);

    // Loop until 'q' is pressed
    char key;
    do {
        key = cv::waitKey(0);  // Wait for 1 ms for a key press
        if (key != -1) {
            std::cout << "Key pressed: " << key << std::endl;
        }
    } while (key != 'q');  // Continue looping until 'q' is pressed

    // Close the window
    cv::destroyWindow("Example");
}
////////////////////////////////////////////////////////////////
// Lab 2
////////////////////////////////////////////////////////////////
void lab2()
{
}
//Lab 3 menu
void lab3Menu() {
    std::cout
        << "****************************************\n"
        << "ELEX4618 LAB 3 MENU\n"
        << "****************************************\n"
        << "(L)ights and buttons! \n"
        << "(d)debounced buttons\n"
        << "(A)nalog Input\n"
        << "(S)ervo\n"
        << "CMD> ";
    /*
    g s

    digital, Analog, servo
    I/O      I      O

    */
}

#define CANVAS_NAME "hiddenCanvas"
void lab3()
{

   // typedef enum {GPIO_PIN2_INPUT = 2, GPIO_PIN3_OUTPUT = 3, GPIO_PIN4_ANALOG = 4};
    int bitRate = 115200;
    CControl controllerInstance;
    int comPort = controllerInstance.init_com(bitRate);
    cv::Mat _hiddenCanvas = cv::Mat::zeros(1, 1, CV_8UC3);
    cvui::init(CANVAS_NAME);
    char key = ' ';
    cv::imshow(CANVAS_NAME,_hiddenCanvas);
    cv::moveWindow(CANVAS_NAME, -100, -100); // These coordinates are off the typical screen area
    cvui::update();

    lab3Menu();
    char cmd;

    std::cin >> cmd;
    cmd = toupper(cmd);

    static bool comVerified = false; // State to remember if COM port is verified
    static float lastVerifyTime = 0;
    float verifyDelay = 1000; // Delay in milliseconds to wait before re-verifying COM port

    /*
    Goal #1 Turn on an LED.

    VIA SET DATA to do that we must initilize....
    */

    while(key != 'q'){
        std::cout << "key: " << key << "\n";
        key = cv::waitKey(100);
        cv::imshow(CANVAS_NAME,_hiddenCanvas);
        switch (cmd) {
            case 'L': {
                int result = 1;
                controllerInstance.lightsAndButtons(controllerInstance.GPIO_PIN2_INPUT,
                                        controllerInstance.GPIO_PIN3_OUTPUT, result);
                break;
            }case 'D': {

                //controllerInstance.get_button(controllerInstance.GPIO_PIN2_INPUT);
                controllerInstance.get_button(2);
                break;
            }case 'E': {
                controllerInstance.get_button(controllerInstance.GPIO_PIN21_INPUT);
                //controllerInstance.get_button(controllerInstance.GPIO_PIN16_INPUT);
                break;
            }case 'A': {
            /*
                int randomChannel = -1;
                controllerInstance.get_analog(controllerInstance.GPIO_PIN4_ANALOG);//what channel....
            */

                cv::Point xyJoystick;
                controllerInstance.analogInputs(controllerInstance.GPIO_PIN4_ANALOG, controllerInstance.GPIO_PIN5_ANALOG, xyJoystick);

                std::cout << "ANALOG TEST: "
                    << "CH" << controllerInstance.GPIO_PIN4_ANALOG << " = " << xyJoystick.x * 4096 / 100
                    << " (" << std::fixed << std::setprecision(1) << xyJoystick.x << "%) "
                    << "CH" << controllerInstance.GPIO_PIN5_ANALOG << " = " << xyJoystick.y * 4096 / 100
                    << " (" << std::fixed << std::setprecision(1) << xyJoystick.y << "%)"
                    << std::endl;

                break;
            }case 'S': {
                //int angle = 500;
                //int maxAngle = 2400;

                int angle = 0;
                int maxAngle = 180;
                //gpioDelay(500000);
                while (angle < maxAngle+1) {

                    std::cout << angle << "\n";
                    //gpioDelay(200000);

                    controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, angle);
                    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
                    angle = (angle + 20 );
                }

                break;
            }default: {
                std::cout << "erm something went wrong \n";
                break;
            }
        }

    }
    gpioTerminate();
    /*
    controllerInstance.set_data(0, 38, 0);
    controllerInstance.set_data(0, 39, 1);
    controllerInstance.set_data(0, 37, 1);

    while (true) {
        //std::cout << "\n ANALOG %: " << controllerInstance.get_analog(11);
        //controllerInstance.get_button(31);
        //std::cout << "\n DIGITAL BUTTON: " <<
    }
    //controllerInstance.get_analog(2);
       */

}

////////////////////////////////////////////////////////////////
// Lab 4
////////////////////////////////////////////////////////////////
void lab4()
{

}

////////////////////////////////////////////////////////////////
// Lab 5
////////////////////////////////////////////////////////////////
void lab5()
{
    cv::Size canvasSize = cv::Size(600, 1000);
    int comPort = 921600;
   // PPong pong(canvasSize, comPort);
    //pong.start();//threading operation
    //pong.run();
}

////////////////////////////////////////////////////////////////
// Lab 6
////////////////////////////////////////////////////////////////
void lab6()
{
}

void print_menu()
{
    std::cout << "\n***********************************";
    std::cout << "\n* ELEX4618 Template Project";
    std::cout << "\n***********************************";
    std::cout << "\n(1) Lab 1 - User Input";
    std::cout << "\n(2) Lab 2 - Grading";
    std::cout << "\n(3) Lab 3 - Embedded Control";
    std::cout << "\n(4) Lab 4 - Etch-A-Sketch";
    std::cout << "\n(5) Lab 5 - Pong";
    std::cout << "\n(6) Lab 6 - Classic Arcade Game";
    std::cout << "\n(7) Lab 7 - Linux Port";
    std::cout << "\n(8) Lab 8 - POTATO CAR! ";
    std::cout << "\n(9) Lab 9 - Sockets";
    std::cout << "\n(10) Test serial COM communication";
    std::cout << "\n(11) Show image manipulation";
    std::cout << "\n(12) Show video manipulation";
    std::cout << "\n(13) Test client/server communication";
    std::cout << "\n(0) Exit";
    std::cout << "\nCMD> ";
}
////////////////////////////////////////////////////////////////
// Demo client server communication
////////////////////////////////////////////////////////////////
void serverthread(CServer* server)
{
  // Start server
  server->start(4618);
}

void do_clientserver()
{
  char inputchar = 0;
  std::vector<std::string> cmds;

  cv::VideoCapture vid;
  CServer server;

  // Start server thread
  std::thread t(&serverthread, &server);
  t.detach();

  vid.open(0);

  while (inputchar != 'q')
  {
    inputchar = cv::waitKey(100);

    server.get_cmd(cmds);
    if (cmds.size() > 0)
    {
      // Process different commands received by the server
      for (int i = 0; i < cmds.size(); i++)
      {
        if (cmds.at(i) == "im")
        {
          std::cout << "\nServer Rx: " << cmds.at(i);

          std::string reply = "Hi there from Server";
          server.send_string(reply);
        }
        else
        {
          std::cout << "\nServer Rx: " << cmds.at(i);

          std::string reply = "Got some other message";
          server.send_string(reply);
        }
      }
    }
		// Update server image with the latest camera image
		if (vid.isOpened() == true)
		{
			cv::Mat frame;
			vid >> frame;
            // Resize the frame to half its original size
            cv::Size newSize(frame.cols / 5, frame.rows / 5);
            cv::resize(frame, frame, newSize);
			if (frame.empty() == false)
			{
				imshow("Server Image", frame);
				//cv::waitKey(1);
				server.set_txim(frame);//sends frame over!
			}
		}
  };

  server.stop();

  sleep(100);
}
////////////////////////////////////////////////////////////////
// Generate ARUCO markers
////////////////////////////////////////////////////////////////
void generate_marks()
{
  std::string str;
  cv::Mat im;
  int mark_size = 250;

  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  for (int marker_id = 0; marker_id < 250; marker_id++)
  {
    cv::aruco::drawMarker(dictionary, marker_id, mark_size, im, 1);
    str = "mark" + std::to_string(marker_id) + ".png";
    cv::imwrite(str, im);
  }
}

void do_video()
{
    cv::VideoCapture vid;

    //vid.open(0, cv::CAP_DSHOW);
    vid.open(0,cv::CAP_V4L2);
    //vid.open(0);
    bool do_canny = true;
    bool do_aruco = false;
    int canny_thresh = 30;
    bool do_exit = false;

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    // initialize GUI system
    cvui::init(CANVAS_NAME);
    cv::Point gui_position;

    std::vector<cv::Scalar> color_vec;
    color_vec.push_back(cv::Scalar(255, 255, 255));
    color_vec.push_back(cv::Scalar(255, 0, 0));
    color_vec.push_back(cv::Scalar(0, 255, 0));
    color_vec.push_back(cv::Scalar(0, 0, 255));
    std::vector<float> areas;
    int color_index = 0;

    if (vid.isOpened()){
        do
        {
            std::cout<<"camera is on !" <<"\n";
            cv::Mat frame, edges;

            // Capture video frame
            vid >> frame;

            // Make sure video frame exists
            if (frame.empty() == false)
            {
                //cv::flip(frame, frame, 0);
                std::vector<int> ids;
                std::vector<std::vector<cv::Point2f> > corners;
                cv::aruco::detectMarkers(frame, dictionary, corners, ids);
                if (ids.size() > 0){
                    for(int i = 0; i < ids.size(); i++){
                        areas.push_back(std::abs( ( (corners[i][0].x * corners[i][1].y - corners[i][0].y * corners[i][1].x )
                        + (corners[i][1].x * corners[i][2].y - corners[i][1].y * corners[i][2].x )
                        + (corners[i][2].x * corners[i][3].y - corners[i][2].y * corners[i][3].x )
                        + (corners[i][3].x * corners[i][0].y - corners[i][3].y * corners[i][0].x )  ) / 2));

                        std::cout << ids[i] << " - " << areas[i] << "\n";
                    }
                    cv::aruco::drawDetectedMarkers(frame, corners, ids);
                }
                // GUI Menu

                gui_position = cv::Point(10, 10);

                //  frame, gui_position.x, gui_position.y, 200, 190, "Video Test");
                gui_position += cv::Point(5, 25);
                cvui::checkbox(frame, gui_position.x, gui_position.y, "Canny Filter", &do_canny);
                gui_position += cv::Point(0, 25);
                cvui::checkbox(frame, gui_position.x, gui_position.y, "ArUco", &do_aruco);
                gui_position += cv::Point(0, 25);
                cvui::text(frame, gui_position.x, gui_position.y, "Canny Threshold");
                gui_position += cv::Point(0, 15);
                cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &canny_thresh, 5, 120);
                gui_position += cv::Point(0, 50);
                if (cvui::button(frame, gui_position.x, gui_position.y, 100, 30, "Colour Switch"))
                {
                    color_index++;
                    if (color_index >= color_vec.size()) { color_index = 0; }
                }
                gui_position += cv::Point(120, 0);
                if (cvui::button(frame, gui_position.x, gui_position.y, 50, 30, "Exit"))
                {
                    do_exit = true;
                }

                // Update the CVUI GUI system
                cvui::update();

                cv::imshow(CANVAS_NAME, frame);
            }
        } while (cv::waitKey(1) != 'q' && do_exit == false);
    }
}


void lab7() {
    //cv::Size canvasSize = cv::Size(500, 900);

    //cv::Size canvasSize = cv::Size(300, 900);
    //cv::Size canvasSize = cv::Size(100, 100);
    //CRecyclingSort gandalf(canvasSize);

    //gandalf.run();
    //gandalf.do_clientserver();
    //gandalf.start();
}
void picar(){
    cv::Size canvasSize = cv::Size(300, 900);
    //cv::Size canvasSize = cv::Size(100, 100);
    CCar potato;
    //gandalf.run();
    //gandalf.do_clientserver();
    potato.drive();
}
int main(int argc, char* argv[])
{
    // generate_marks();

    int cmd = -1;
    do
    {
        print_menu();
        std::cin >> cmd;
        switch (cmd)
        {

        case 1: lab1(); break;
        case 2: lab2(); break;
        case 3: lab3(); break;
        case 4: lab4(); break;
        case 5: lab5(); break;
        case 6: lab6(); break;
        case 7: lab7(); break;
        case 8: picar(); break;
            //        case 10: test_com(); break;
            //          case 11: do_image(); break;
        case 12: do_video(); break;
        case 13: do_clientserver(); break;
        }
    } while (cmd != 0);
}

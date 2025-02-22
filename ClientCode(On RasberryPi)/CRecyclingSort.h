#pragma once
#include "CBase4618.h"
#include "CControler.h"
#include <opencv2/highgui.hpp> // For imshow
#include <thread>
#include "Client.h"
#include "server.h"

class CRecyclingSort : public CBase4618
{
public:
	CRecyclingSort(cv::Size canvasSize);
    ~CRecyclingSort();

    void do_clientserver();

	void start();
private:

	void draw() override;
	void update() override;
	void drawGUI();
    void drawWirelessGUI();
	bool detectGreen();
	void detectColor();
	bool detectBlue();
	bool detectPink();
	bool detectYellow();

	void throwBin(int bin);
    bool isStarted;

	void servoControlSLow(int newAngle, int subsections, int timeDelay);
	char key; ///< Key input for controlling the game flow.
	int color_index;
	cv::Size matSize;
	cv::VideoCapture vid;

	int hue;
	int saturation;
	int value;

	int iLowH ;
	int iHighH;

	int iLowS ;
	int iHighS;

	int iLowV ;
	int iHighV ;

	int iLastX;
	int iLastY;

	int bin;
	int _ball1Count;
	int _ball2Count;
    CControl controllerInstance;
    int center;

    cv::Point rectSize;
    cv::Point rectCenter;
    cv::Rect roiRect;
    cv::Scalar rectColor ; // Color (Green)
    int rectThickness; // Thickness of the rectangle lines

    enum ledStateTHING { ledON = 0, ledOFF };//need to add into get_Data
    enum typeEnum { DIGITAL = 0, ANALOG, SERVO };//need to add into get_Data

    int autoButton;
    int bin1Button;
    int bin2Button;

    int prevButton;
    int prevbin1Button ;
    int prevbin2Button;

    int colorCountVerify;
    std::string lastColor;
    /*FPS STUFF*/
    std::chrono::steady_clock::time_point calc_start; ///< Start time point for frame calculation.
    std::chrono::steady_clock::time_point calc_end; ///< End time point for frame calculation.
    std::chrono::milliseconds deltaT; ///< Time difference for frame rate calculation.


    /*THREAD and SERVER STUFF*/
    CServer server;
    CServer _outputserver;
    static void serverThread(CServer* server);//, int port
    static void vidThread(CServer* server);//, int port

    static void updateThread(CRecyclingSort* ptr);
    static void drawThread(CRecyclingSort* ptr);
    // void outputServerThread(CRecyclingSort* ptr);
    std::vector<std::string> cmds;

    /*Threading stuff*/
    void end();

    bool isThreading;

    std::thread draw_thread; ///< Thread for handling camera.
    std::thread update_thread; ///< Thread for handling camera.
    std::thread server_thread; ///< Thread for handling camera.
    //std::thread outputServer_thread;

    std::thread vid_thread;


    std::mutex vid_mutex;
    /*End of threading stuff*/

};


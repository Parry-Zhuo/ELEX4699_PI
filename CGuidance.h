#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp> // For imshow
#include <vector>
#include <thread>
#include <opencv2/aruco.hpp>

#include "Client.h"
#include "server.h"
//#include "raspicam_cv.h"
class CGuidance
{
private:

    cv::VideoCapture vid;
    cv::Size matSize;
    std::chrono::steady_clock::time_point calc_start; ///< Start time point for frame calculation.
    std::chrono::steady_clock::time_point calc_end; ///< End time point for frame calculation.
    std::chrono::milliseconds deltaT; ///< Time difference for frame rate calculation.
    void drawGUI();
    cv::Ptr<cv::aruco::Dictionary> dictionary;

public:

	CGuidance();
	~CGuidance();
	void update();///i'm guessing this does all the calculations
	void get_im();///this one would probably be in charge of displaying image?

    char key;

    bool isThreading;
    static void imageThread(CGuidance* ptr);
    static void updateThread(CGuidance* ptr);
    std::thread _imThread; ///< Thread for handling game drawing.
    cv::Mat _canvas;
    bool isShoot = false;

    /*THREAD and SERVER STUFF*/
    CServer server;
    CServer _outputserver;
    static void serverThread(CServer* server);//, int port
    static void vidThread(CServer* server);//, int port

    std::vector<std::string> cmds;
    std::mutex vid_mutex;

};

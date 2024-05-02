#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp> // For imshow

#include <vector>
#include <thread>

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

public:

	CGuidance();
	~CGuidance();
	void update();///i'm guessing this does all the calculations
	void get_im();///this one would probably be in charge of displaying image?

    char key;
    bool isThreading;
    static void imageThread(CGuidance* ptr);
    std::thread _imThread; ///< Thread for handling game drawing.
    cv::Mat _canvas;
};

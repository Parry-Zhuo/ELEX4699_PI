#pragma once
#include <opencv2/opencv.hpp>


#include <vector>
#include <thread>
//#include "raspicam_cv.h"
class CGuidance
{
private:
	cv::Mat _display_im;
	cv::VideoCapture _cap;

public:

	CGuidance();
	~CGuidance();
	void update();///i'm guessing this does all the calculations
	void get_im(cv::Mat& im);///this one would probably be in charge of displaying image?

    std::chrono::steady_clock::time_point calc_start; ///< Start time point for frame calculation.
    std::chrono::steady_clock::time_point calc_end; ///< End time point for frame calculation.
    std::chrono::milliseconds deltaT; ///< Time difference for frame rate calculation.
    char key;
};

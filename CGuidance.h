#include <opencv2/opencv.hpp>
#include "raspicam_cv.h"
class CGuidance
{
private:
	cv::Mat _display_im;
	cv::VideoCapture _cap;
public:
	CGuidance();
	~CGuidance();
	void update();
	void get_im(cv::Mat& im);
};
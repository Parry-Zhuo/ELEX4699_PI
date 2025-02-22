
/*
A base class will be created that will hold common code used in all of the following labs for this course.
The CBase4618 class should only have two member variables at this time, the CControl object and an
OpenCV image object of type Mat called _canvas upon which to draw.
Add pure virtual methods called update and draw which will be implemented in the in the child class
below. Add a run method which calls update and draw in a loop while waiting for the user to press ‘q’ to
quit. If the user presses ‘q’ the program should exit, you can use cv::waitKey but do not use anywhere
else in your program or the ‘q’ key may not be caught properly.

*/

#include "CControler.h"
#include "CBase4618.h"
#include <opencv2/highgui.hpp>

CBase4618::CBase4618() {
    // Initialize _canvas, for example with a size and type
    _canvas = cv::Mat::zeros(480, 640, CV_8UC3); // Example size and type
}

CBase4618::~CBase4618() {
    // Destructor content if needed
}


void CBase4618::run() {
    while (true) {

        update();
        draw();

        // cv::waitKey waits for a key press for the specified duration (in milliseconds).
        // If the duration is 0, it waits indefinitely, but here we use 1 millisecond
        // to ensure the loop continues to run smoothly, allowing for UI responsiveness.

    }
}


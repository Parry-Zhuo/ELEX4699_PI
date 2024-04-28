#include "CGuidance.h"

#define CANVAS_NAME "POTATO CAMERA DISPLAY  "

/*
 * @file CGuidance.h
 * @brief
read the camera image
identify the position of an ArUCO markers.
Determine the action needed to progress (forward, left, right, etc).
Any other sensors may be integrated in this class.
The image captured and processed in CGuidance should be copied into the
CCommunication object for transmission to the client.
*/
CGuidance::CGuidance() {
    // Constructor implementation

     cv::Size canvasSize = cv::Size(300, 900);
    _display_im = cv::Mat::zeros(canvasSize.width/2, canvasSize.height/2, CV_8UC3);
    key = ' ';
   // cvui::init(CANVAS_NAME);
 //   matSize = _canvas.size();

    /*Initialize video*/
    /*
    vid.open(0, cv::CAP_V4L2);
    if (!vid.isOpened()){
        std::cout << "Unable to open camera" << std::endl;
        exit(0);
    }
    */
}

CGuidance::~CGuidance() {
    // Destructor implementation
}
/*What's the purpose of this function FOR PHASE ONE this does nothing.*/
void CGuidance::update(){

}
/*What's the purpose of this function*/
void CGuidance::get_im(cv::Mat& im){
//probably here.
//    cvui::update();

    cv::imshow(CANVAS_NAME, im);
    key = cv::waitKey(1);
    std::cout<< "key "<< key << "\n";
    std::this_thread::sleep_until(calc_start + std::chrono::milliseconds(30));
    calc_end = std::chrono::steady_clock::now();
    deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start) * 2;
    calc_start = std::chrono::steady_clock::now();

}

//#include "stdafx.h"
#include "CRecyclingSort.h"

//#include "CControler.h"
//#include "PPong.h"
#include <opencv2/highgui.hpp>
#include "cvui.h"
#include <vector>
#include <thread>
//#include "CSketch.h"


#define CANVAS_NAME "potato"


/*Sources

CHATGPT WAS USED IN THE CREATION OF THIS PROGRAM

//https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html?m=1
THis program was developed using ADVANCED machine learning algorithms to sort colour of balls.
*/


CRecyclingSort::CRecyclingSort(cv::Size canvasSize) {
    _canvas = cv::Mat::zeros(canvasSize.width/2, canvasSize.height/2, CV_8UC3);
    cvui::init(CANVAS_NAME);
    matSize = _canvas.size();

    /*Initialize video*/
    vid.open(0, cv::CAP_V4L2);
    if (!vid.isOpened()){
        std::cout << "Unable to open camera" << std::endl;
        exit(0);
    }
    int bitRate = 115200;
    //CControl controllerInstance;
    _control.init_com(bitRate);
    vid.set(cv::CAP_PROP_FRAME_WIDTH, matSize.width);
    vid.set(cv::CAP_PROP_FRAME_HEIGHT, matSize.height);

    isThreading = false;

    key = ' ';
    hue = 0;
    saturation = 0;
    value = 0;
    iLowH = 170;
    iHighH = 179;
    iLowS = 0;
    iHighS = 255;
    iLowV = 0;
    iHighV = 255;
    bin = 2;
    center = 115;

    _ball1Count = 0;
    _ball2Count = 0;

    rectSize = cv::Point(250, 80);
    rectCenter = cv::Point(matSize.width / 2  , matSize.height  - rectSize.y/2 );
    roiRect= cv::Rect(rectCenter.x - rectSize.x / 2, rectCenter.y - rectSize.y / 2, rectSize.x, rectSize.y);
    rectColor = cv::Scalar(0, 255, 0); // Color (Green)
    rectThickness = 2; // Thickness of the rectangle lines
    isStarted = false;



    autoButton = controllerInstance.get_button(controllerInstance.GPIO_PIN2_INPUT);
    bin1Button = controllerInstance.get_button2(controllerInstance.GPIO_PIN16_INPUT);
    bin2Button = controllerInstance.get_button3(controllerInstance.GPIO_PIN20_INPUT);

    prevButton = autoButton;
    prevbin1Button = bin1Button;
    prevbin2Button = bin2Button;

    int colorCountVerify = 0;
    std::string lastColor = "none";


    /*@@@@@@INTEGRATION OF SERVER/CLIENT OCCURS HERE*/
    calc_start = std::chrono::steady_clock::now();
    calc_end = calc_start;
    //calc_collisionTime = calc_end;
    deltaT = std::chrono::milliseconds(1);
}
CRecyclingSort::~CRecyclingSort(){
    server.stop();
    end();
}
void CRecyclingSort::drawWirelessGUI() {
    float fps = 1000.0f / deltaT.count(); // Convert ms to seconds, then take the reciprocal for FPS
    cv::Mat frame = _canvas.clone();
    cv::Point2f gui_position = cv::Point2f(20, 20);

        // Saturation trackbar with label

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << fps*2;
    std::string fpsString = "FPS: " + stream.str();
    cvui::text(frame, gui_position.x, gui_position.y, fpsString);
    gui_position += cv::Point2f(0, 30);
    if (bin != -1) {
        cvui::text(frame, gui_position.x, gui_position.y, "bin " + std::to_string(bin));
    }
    else {
        cvui::text(frame, gui_position.x, gui_position.y, "no bin ");
    }
    gui_position.y = 50; // Adjust spacing for the next trackbar

    if ((cvui::button(frame, gui_position.x , matSize.height - 25, 100, 30, "EXIT") || key == 'q')) {//  || key == 'Q'
        char key = 'q';
        //end();
        exit(0);
    }
    //server.set_txim(frame);
    frame.copyTo(_canvas);
}
void CRecyclingSort::drawGUI() {
    cv::Mat frame = _canvas.clone();
    cv::Point2f gui_position = cv::Point2f(20, 20);

    // Hue trackbar with label
    cvui::text(frame, gui_position.x, gui_position.y, "High Hue Range");
    gui_position.y += 20; // Adjust for label spacing
    cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &iHighH, 0, 180, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1);
    gui_position.y += 50; // Adjust spacing for the next trackbar

    cvui::text(frame, gui_position.x, gui_position.y, "Low Hue Range");
    gui_position.y += 20; // Adjust for label spacing
    cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &iLowH, 0, 180, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1);
    gui_position.y += 50; // Adjust spacing for the next trackbar
/*
    // High Saturation Range
    cvui::text(frame, gui_position.x, gui_position.y, "High Saturation Range");
    gui_position.y += 20; // Adjust for label spacing
    cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &iHighS, 0, 255, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1);
    gui_position.y += 50; // Adjust spacing for the next trackbar

    // Low Saturation Range
    cvui::text(frame, gui_position.x, gui_position.y, "Low Saturation Range");
    gui_position.y += 20; // Adjust for label spacing
    cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &iLowS, 0, 255, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1);
    gui_position.y += 50; // Adjust spacing for the next trackbar

    // High Value Range
    cvui::text(frame, gui_position.x, gui_position.y, "High Value Range");
    gui_position.y += 20; // Adjust for label spacing
    cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &iHighV, 0, 255, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1);
    gui_position.y += 50; // Adjust spacing for the next trackbar

    // Low Value Range
    cvui::text(frame, gui_position.x, gui_position.y, "Low Value Range");
    gui_position.y += 20; // Adjust for label spacing
    cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &iLowV, 0, 255, 1, "%.0Lf", cvui::TRACKBAR_DISCRETE, 1);
    gui_position.y += 50; // Ready for any additional UI elements
*/
        // Saturation trackbar with label
    if (bin != -1) {
        cvui::text(frame, gui_position.x, gui_position.y, "bin " + std::to_string(bin));
    }
    else {
        cvui::text(frame, gui_position.x, gui_position.y, "no bin ");
    }
    gui_position.y = 50; // Adjust spacing for the next trackbar


    /*Placement of the 4 BUTTONS!*/

    if ((cvui::button(frame, gui_position.x , matSize.height - 150, 100, 30, "START") )) {//  || key == 'Q'
        isStarted = true;
    }if ((cvui::button(frame, gui_position.x+  100, matSize.height - 150, 100, 30, "STOP") )) {//  || key == 'Q'
        isStarted = false;
    }if ((cvui::button(frame, gui_position.x+  200, matSize.height - 150, 100, 30, "SORT MIDDLE ") )) {//  || key == 'Q'
        throwBin(0);
    }if ((cvui::button(frame, gui_position.x, matSize.height - 100, 100, 30, "SORT BIN 1"))) {//
        throwBin(1);
    }if ((cvui::button(frame, gui_position.x+  100, matSize.height - 100, 100, 30, "SORT BIN 2"))) {//  || key == 'Q'
        std::cout<< "bin 2: "<< "\n";
        servoControlSLow(-80,50,400);
    }if ((cvui::button(frame, gui_position.x+  200, matSize.height - 100, 100, 30, "SORT BIN 3"))) {
        //throwBin(2);
        std::cout<< "technically bin 4: "<< "\n";
        servoControlSLow(60,25,100);

    }if ((cvui::button(frame, gui_position.x+  300, matSize.height - 100, 100, 30, "SORT BIN 4"))) {
        throwBin(4);
    }if ((cvui::button(frame, gui_position.x , matSize.height - 25, 100, 30, "EXIT") || key == 'q')) {//  || key == 'Q'
        char key = 'q';
        //end();
        exit(0);
    }
    //server.set_txim(frame);
    frame.copyTo(_canvas);
}
//https://stackoverflow.com/questions/46187563/finding-largest-contours-c#:~:text=To%20compare%20two%20contours%20you,contourArea()%20as%20mentioned%20before.&text=And%20then%20you%20find%20your,at(getMaxAreaContourId(contours))%20.
int getMaxAreaContourId(std::vector <std::vector<cv::Point>> contours) {
    double maxArea = 200;
    int maxAreaContourId = -1;
    for (int j = 0; j < contours.size(); j++) {
        double newArea = cv::contourArea(contours.at(j));
        if (newArea > maxArea) {
            maxArea = newArea;
            maxAreaContourId = j;
            return maxAreaContourId;
        } // End if
    } // End for
    return maxAreaContourId;
} // End function
bool CRecyclingSort::detectYellow() {
    int blue_iHighH = 43;
    int blue_iLowH = 28;
    int blue_iHighS = 109;
    int blue_iLowS = 44;
    int blue_iHighV = 185;
    int blue_iLowV = 143;


    cv::Mat _newCanvas;
    cv::Mat _subCanvas;
    _canvas(roiRect).copyTo(_subCanvas);
    cvtColor(_subCanvas, _newCanvas, cv::COLOR_BGR2HSV);//conversion to from BGR to HSV so OPENCV can read
    cv::Mat imgThresholded;

    inRange(_newCanvas, cv::Scalar(blue_iLowH, blue_iLowS, blue_iLowV), cv::Scalar(blue_iHighH, blue_iHighS, blue_iHighV), imgThresholded); //Threshold the image

    erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));//erodes the edges of the stuff
    dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3))); // Adds pixels to edges

    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> contour;

//get_im();
    cv::findContours(imgThresholded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int maxAreaContourIdx = getMaxAreaContourId(contours);

    if (maxAreaContourIdx != -1) {
        double maxArea = cv::contourArea(contours.at(maxAreaContourIdx));

        cv::drawContours(imgThresholded, contours, maxAreaContourIdx, cv::Scalar(255, 255, 255), cv::FILLED, 8, hierarchy);
      //  std::cout << "max " << maxArea << "\n";
        cv::Rect r = cv::boundingRect(contours[maxAreaContourIdx]);
        return true;
    }
    else {
        return false;
    }
    imshow("Yellowthreshold Image", imgThresholded);

}
bool CRecyclingSort::detectPink() {
    int blue_iHighH = 180;
    int blue_iLowH = 156;
    int blue_iHighS = 218;
    int blue_iLowS = 107;
    int blue_iHighV = 221;
    int blue_iLowV = 128;


    cv::Mat _newCanvas;
    cv::Mat _subCanvas;
    _canvas(roiRect).copyTo(_subCanvas);
    cvtColor(_subCanvas, _newCanvas, cv::COLOR_BGR2HSV);//conversion to from BGR to HSV so OPENCV can read
    cv::Mat imgThresholded;

    inRange(_newCanvas, cv::Scalar(blue_iLowH, blue_iLowS, blue_iLowV), cv::Scalar(blue_iHighH, blue_iHighS, blue_iHighV), imgThresholded); //Threshold the image

    erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));//erodes the edges of the stuff
    dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3))); // Adds pixels to edges

    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> contour;


    cv::findContours(imgThresholded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int maxAreaContourIdx = getMaxAreaContourId(contours);

    if (maxAreaContourIdx != -1) {
        double maxArea = cv::contourArea(contours.at(maxAreaContourIdx));

        cv::drawContours(imgThresholded, contours, maxAreaContourIdx, cv::Scalar(255, 255, 255), cv::FILLED, 8, hierarchy);
       // std::cout << "max " << maxArea << "\n";
        cv::Rect r = cv::boundingRect(contours[maxAreaContourIdx]);
        return true;
    }
    else {
        return false;
    }
    imshow("Pinkthreshold Image", imgThresholded);
//update
}
bool CRecyclingSort::detectBlue(){

    int blue_iHighH = 107;
    int blue_iLowH = 88;
    int blue_iHighS = 255;
    int blue_iLowS = 121;
    int blue_iHighV = 173;
    int blue_iLowV = 117;

    cv::Mat _newCanvas;
    cv::Mat _subCanvas;
    _canvas(roiRect).copyTo(_subCanvas);
    cvtColor(_subCanvas, _newCanvas, cv::COLOR_BGR2HSV);//conversion to from BGR to HSV so OPENCV can read
    cv::Mat imgThresholded;

    inRange(_newCanvas, cv::Scalar(blue_iLowH, blue_iLowS, blue_iLowV), cv::Scalar(blue_iHighH, blue_iHighS, blue_iHighV), imgThresholded); //Threshold the image

    erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));//erodes the edges of the stuff
    dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3))); // Adds pixels to edges

    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> contour;


    cv::findContours(imgThresholded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int maxAreaContourIdx = getMaxAreaContourId(contours);

    if (maxAreaContourIdx != -1) {
        double maxArea = cv::contourArea(contours.at(maxAreaContourIdx));

        cv::drawContours(imgThresholded, contours, maxAreaContourIdx, cv::Scalar(255, 255, 255), cv::FILLED, 8, hierarchy);
      //  std::cout << "max " << maxArea << "\n";
        cv::Rect r = cv::boundingRect(contours[maxAreaContourIdx]);
        return true;
    }
    else {
        return false;
    }
    imshow("BlueThresholded Image", imgThresholded);

}
void CRecyclingSort::detectColor() {
    cv::Mat _newCanvas;
    cvtColor(_canvas, _newCanvas, cv::COLOR_BGR2HSV);//conversion to from BGR to HSV so OPENCV can read
    cv::Mat imgThresholded;

    inRange(_newCanvas, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

    erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));//erodes the edges of the stuff
    dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3))); // Adds pixels to edges

    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> contour;


    cv::findContours(imgThresholded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int maxAreaContourIdx = getMaxAreaContourId(contours);

    if (maxAreaContourIdx != -1) {
        double maxArea = cv::contourArea(contours.at(maxAreaContourIdx));

        cv::drawContours(imgThresholded, contours, maxAreaContourIdx, cv::Scalar(255, 255, 255), cv::FILLED, 8, hierarchy);
       // std::cout << "max " << maxArea << "\n";
        cv::Rect r = cv::boundingRect(contours[maxAreaContourIdx]);
     //   bin = 1;
    }
    else {
     //   bin = -1;
    }
    imshow("Thresholded Image", imgThresholded);

}
void CRecyclingSort::throwBin(int bin){
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(50));
    if(bin == 1){
        _ball1Count++;
        controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, center);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
        controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, center - 65);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(400));
    }else if(bin == 4){
        _ball2Count++;
        controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, center);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
        controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, center + 65);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(400));
    }else if (bin == 0){// sort to center
        controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, center );
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(400));
    }else if (bin == 2){
        servoControlSLow(40,10,800);
    }

}
bool CRecyclingSort::detectGreen() {

    int green_iHighH = 86;
    int green_iLowH = 49;
    int green_iHighS = 235;
    int green_iLowS = 89;
    int green_iHighV = 171;
    int green_iLowV = 117;

    cv::Mat _newCanvas;
    cv::Mat _subCanvas;
    _canvas(roiRect).copyTo(_subCanvas);
    cvtColor(_subCanvas, _newCanvas, cv::COLOR_BGR2HSV);//conversion to from BGR to HSV so OPENCV can read
    cv::Mat imgThresholded;
    cv::Mat green_imgThresholded;

    inRange(_newCanvas, cv::Scalar(green_iLowH, green_iLowS, green_iLowV), cv::Scalar(green_iHighH, green_iHighS, green_iHighV), green_imgThresholded); //detect green

    erode(green_imgThresholded, green_imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));//erodes the edges of the stuff
    dilate(green_imgThresholded, green_imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3))); // Adds pixels to edges


    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> contour;

    cv::findContours(green_imgThresholded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int green_maxAreaContourIdx = getMaxAreaContourId(contours);

    if (green_maxAreaContourIdx != -1) {
        double maxArea = cv::contourArea(contours.at(green_maxAreaContourIdx));

        cv::drawContours(green_imgThresholded, contours, green_maxAreaContourIdx, cv::Scalar(255, 255, 255), cv::FILLED, 8, hierarchy);
        //std::cout << "max " << maxArea << "\n";
        cv::Rect r = cv::boundingRect(contours[green_maxAreaContourIdx]);

        return true;
    }
    else {

        return false;
    }
    imshow("green_Thresholded Image", green_imgThresholded);
}
//new angle refers to angle offset from center.
void CRecyclingSort::servoControlSLow(int newAngle, int subsections, int timeDelayMilliseconds){
  //                  controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, center - 85);
    //                std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));

                int angle = center;
                int maxAngle = newAngle;
                angle = 0;
                int deltaAngle = (center - maxAngle)/subsections;

                std::cout<< "maxAngle: " << maxAngle<< " deltaAngle: "<< deltaAngle<<  " subsection: "<< subsections<<  "\n";

                while (angle < maxAngle+1 || angle < -1*(maxAngle+1)) {

                    std::cout << angle << "\n";
                    //gpioDelay(200000);
                    if(newAngle > 0){
                        controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, (center+angle) );
                    }else{
                        controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, (center-angle) );
                    }
                    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(timeDelayMilliseconds));
                    angle = (angle + deltaAngle );
                }

}
void CRecyclingSort::end() {
    if (update_thread.joinable()) {
        update_thread.join();
    }
    if (draw_thread.joinable()) {
        draw_thread.join();
    }if(server_thread.joinable()){
        server_thread.join();
    }
    isThreading = false;
    //_thread_exit = true;
    server.stop();

    exit(0);
}

void CRecyclingSort::draw() {
    // Update the CVUI GUI system
    if (vid.isOpened()) {

        //std::cout << "\n Video has opeend \n";
    //detectColor();
        //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(3000));
        rectCenter = cv::Point(matSize.width / 2  , matSize.height  - rectSize.y/2 );
        //vid_mutex.unlock();
        //cv::resize(_canvas, _canvas, cv::Size(matSize.width/3, matSize.height/3));
        roiRect= cv::Rect(rectCenter.x - rectSize.x / 2, rectCenter.y - rectSize.y / 2, rectSize.x, rectSize.y);
        cv::rectangle(_canvas, roiRect, rectColor, rectThickness);


        //cv::Mat frame, edges;//variables are not currently in use.
        //vid >> _canvas;

        matSize = _canvas.size();

        vid_mutex.lock();

       // std::string reply = "Hi there from Server";
       // server.send_string(reply);

        vid >> _canvas;
        key = cv::waitKey(1);
        if(key == 'q'){
            isThreading = false;
        }
        drawWirelessGUI();
        cvui::update();
        cv::imshow(CANVAS_NAME, _canvas);
        server.set_txim(_canvas);
        vid_mutex.unlock();

        std::this_thread::sleep_until(calc_start + std::chrono::milliseconds(60));
        calc_end = std::chrono::steady_clock::now();
        deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start) ;
        calc_start = std::chrono::steady_clock::now();
    }else{
        std::cout<< "Camera is not openned\n";
        exit(0);
    }


}

void CRecyclingSort::update() {
    //std::cout<<"updatethread\n";

    autoButton = controllerInstance.get_button(controllerInstance.GPIO_PIN2_INPUT);
    bin1Button = controllerInstance.get_button2(controllerInstance.GPIO_PIN16_INPUT);
    bin2Button = controllerInstance.get_button3(controllerInstance.GPIO_PIN20_INPUT);
    //controllerInstance.set_data(DIGITAL, controllerInstance.GPIO_PIN3_OUTPUT,autoButton);
    controllerInstance.set_data(DIGITAL, controllerInstance.GPIO_PIN3_OUTPUT,isStarted);
    //std::cout<< "is started" << isStarted << "\n";
        //isStarted = 0;
    if(prevButton != autoButton ){
        isStarted ^= 1;

        prevButton = autoButton;
    }else if(prevbin1Button != bin1Button){
        std::cout<< "bin 1 \n";
        throwBin(1);
        prevbin1Button = bin1Button;
    }else if(prevbin2Button != bin2Button){
        std::cout<< "bin 2 \n";
        throwBin(4);
        prevbin2Button = bin2Button;
    }


    if (_canvas.empty() == false && isStarted == true) {
        if (detectGreen()) {
            throwBin(1);
            bin = 1;
            std::cout << "Bin" << bin << "\n";
        }
        else if (detectBlue() || detectPink() || detectYellow()) {
            throwBin(4);
            std::cout << "Bin" << bin << "\n";
            bin = 2;
        }
        else {
            bin = -1;
            controllerInstance.servoControl(controllerInstance.GPIO_PIN4_ANALOG, center);
            std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
            //std::cout << "Nothing " << "\n";
        }
    }

    //server.send_string(std::to_string(_ball2Count));

    if (vid.isOpened()) {
        //vid_mutex.lock();
            //@@@THREADING STUFF
        server.get_cmd(cmds);
        if (cmds.size() > 0)
        {

          // Process different commands received by the server
          for (int i = 0; i < cmds.size(); i++)
          {

            //server.send_string("random word  - test response");
            std::cout << "\n Rx: " << cmds.at(i);
            if (cmds.at(i) == "im")
            {
              std::cout << "\nServer Rx: " << cmds.at(i);

              //std::string reply = "Hi there from Server";

            }else if((cmds.at(i) == "s 0 1 \n") || cmds.at(i) == "s 0 0 \n"){
                isStarted ^=1;
                std::string reply = "Toggled" + std::to_string(isStarted) ;
                //server.send_string(reply);
                std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
            }else if((cmds.at(i) == "s 1 1 \n") ){//sort Bin1
                throwBin(1);
                bin = 1;
            }else if((cmds.at(i) == "s 1 2 \n") ){//sort Bin2
                throwBin(4);
                bin = 2;
            }else if((cmds.at(i

            ) == "g 1 1 \n") ){//get Bin1
                //vid_mutex.lock();
                //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
                std::cout << "Get bin 1 " << std::to_string(_ball1Count) << "\n";
                server.send_string(std::to_string(_ball1Count));
                //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
                //vid_mutex.unlock();
            }else if((cmds.at(i) == "g 1 2 \n") ){//get Bin2
                //vid_mutex.lock();
                //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
                std::cout << "Get bin 2 " << std::to_string(_ball2Count) << "\n";
                server.send_string(std::to_string(_ball2Count));
                //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(1000));
                //vid_mutex.unlock();
            }else{
              std::cout << "\nServer Rx: " << cmds.at(i);

              std::string reply = "Got some other message";

            }
          }
        }


        //vid_mutex.unlock();
        //@@@@ THREADING STUFF
        ////sends frame over!

   }
}
void CRecyclingSort::drawThread(CRecyclingSort* ptr) {
    while (ptr->isThreading) {
        ptr->draw();
    }
}
void CRecyclingSort::updateThread(CRecyclingSort* ptr) {
    while (ptr->isThreading) {
        ptr->update();
    }
}
void CRecyclingSort::vidThread(CServer* server)//
{
  // Start server
  server->start(1234);

}
void CRecyclingSort::serverThread(CServer* server)//
{
  // Start server
  server->start(4618);

}

void CRecyclingSort::start(){
    char inputchar = 0;

    isThreading = true;

    update_thread = std::thread(&CRecyclingSort::updateThread, this);
    draw_thread = std::thread(&CRecyclingSort::drawThread, this);
    server_thread = std::thread(&CRecyclingSort::serverThread,&server);
    vid_thread = std::thread(&CRecyclingSort::vidThread,&server);

    //server_thread(&CRecyclingSort::serverThread, this, &server, 4618);
    //server_thread(&CRecyclingSort::serverThread, this, &server, 4618);

   // outputServer_thread = std::thread(&CRecyclingSort::serverThread, &server,4618);
    update_thread.join();
    draw_thread.join();

    server_thread.detach();
    vid_thread.detach();
    //update_thread.detach();
    //draw_thread.detach();



//inputchar != 'q'

    while (isThreading == true){
        //inputchar = cv::waitKey(1);
        //draw();
        if(inputchar == 'q'){
            isThreading = false;

        }
    }
    end();
    server.stop();
    sleep(100);//not sure why this is here but okay

}

void CRecyclingSort::do_clientserver()
{
  char inputchar = 0;
  //std::vector<std::string> cmds;

  //cv::VideoCapture vid;
  //CServer server;

  // Start server thread
  std::thread t(&serverThread, &server);
  t.detach();

  //vid.open(0);

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

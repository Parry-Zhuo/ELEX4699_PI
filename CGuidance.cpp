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

     matSize = cv::Size(300, 900);
    _canvas = cv::Mat::zeros(matSize.width, matSize.height, CV_8UC3);
    key = ' ';
   // cvui::init(CANVAS_NAME);
 //   matSize = _canvas.size();

    /*Initialize video*/
    vid.open(0, cv::CAP_V4L2);
    if (!vid.isOpened()){
        std::cout << "Unable to open camera" << std::endl;
        exit(0);
    }
    calc_start = std::chrono::steady_clock::now();
    calc_end = calc_start;
    deltaT = std::chrono::milliseconds(1);
    //isThreading = true;
    isThreading = true;
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

}

CGuidance::~CGuidance() {
    // Destructor implementation
}
/*What's the purpose of this function FOR PHASE ONE this does nothing.*/
void CGuidance::update(){

}
void CGuidance::imageThread(CGuidance* ptr) {
    std::cout<< "Getting image " << ptr->isThreading;
    while (ptr->isThreading) {
        ptr->get_im();

    }
}
void CGuidance::get_im(){
    if (vid.isOpened()) {

        cv::Mat edges;
        vid >> _canvas;
        /*
        //@@WARNING
        //ERROR SHUT DOWN OF PI OCCURS SOMETIMES WHEN SERVO MOVES ALONGSIDE THIS
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f> > corners;
        cv::aruco::detectMarkers(_canvas, dictionary, corners, ids);
        if (ids.size() > 0)
        {
            std::cout<< "ID SIZE" << " " << ids.size();
            cv::aruco::drawDetectedMarkers(_canvas, corners, ids);
        }
        */
        //drawGUI();
        cv::imshow(CANVAS_NAME, _canvas);//ensure this is always behind key
        key = cv::waitKey(1);
        //_canvas.copyTo(im);// tbh, not too sure why we need im
        if(key == 'q'){
            isThreading = false;
        }

        //server.set_txim(im);
        //vid_mutex.unlock();
        std::this_thread::sleep_until(calc_start + std::chrono::milliseconds(30));
        calc_end = std::chrono::steady_clock::now();
        deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start) ;
        calc_start = std::chrono::steady_clock::now();
    }else{
        std::cout<< "Camera is not openned\n";
        exit(0);
    }
}
void CGuidance::drawGUI() {
    float fps = 1000.0f / deltaT.count(); // Convert ms to seconds, then take the reciprocal for FPS
    cv::Mat frame = _canvas.clone();
    cv::Point2f gui_position = cv::Point2f(20, 20);

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(0) << fps*2;
    std::string fpsString = "FPS: " + stream.str();

    //cvui::text(frame, , gui_position.y, fpsString);
    cv::putText(frame, fpsString, gui_position, cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
    gui_position += cv::Point2f(0, 30);

    gui_position.y = 50; // Adjust spacing for the next trackbar


    //server.set_txim(frame);
    frame.copyTo(_canvas);
}

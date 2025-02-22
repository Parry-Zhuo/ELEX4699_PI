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

     matSize = cv::Size(320, 240);
    _canvas = cv::Mat::zeros(matSize.width, matSize.height, CV_8UC3);
    frame = cv::Mat::zeros(matSize.width, matSize.height, CV_8UC3);
    key = ' ';
   // cvui::init(CANVAS_NAME);
 //   matSize = _canvas.size();

    /*Initialize video*/
    vid.open(0, cv::CAP_V4L2);
    //vid.open(0,cv::CAP_DSHOW);
    if (!vid.isOpened()){
        std::cout << "Unable to open camera" << std::endl;
        exit(0);
    }
    vid.set(cv::CAP_PROP_FRAME_WIDTH, matSize.width);  // Set the width
    vid.set(cv::CAP_PROP_FRAME_HEIGHT, matSize.height);  // Set the height
    vid.set(cv::CAP_PROP_FPS, 30);  // Set desired FPS
    calc_start = std::chrono::steady_clock::now();
    calc_end = calc_start;
    deltaT = std::chrono::milliseconds(1);
    //isThreading = true;
    isThreading = true;
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    // Initialize the vector with strings
    commandList = {"w", "s", "a", "d", "z", "x", "1", "2", "3", "4", "j", "k", "l", "c", "e"};

}

CGuidance::~CGuidance() {
    // Destructor implementation
}
/*What's the purpose of this function FOR PHASE ONE this does nothing.*/
void CGuidance::update(){
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f> > corners;
    vid >> frame;

    cv::aruco::detectMarkers(frame, dictionary, corners, ids);
    if (ids.size() > 0){
        for(int i = 0; i < ids.size(); i++){
            areas.push_back(std::abs( ( (corners[i][0].x * corners[i][1].y - corners[i][0].y * corners[i][1].x )
            + (corners[i][1].x * corners[i][2].y - corners[i][1].y * corners[i][2].x )
            + (corners[i][2].x * corners[i][3].y - corners[i][2].y * corners[i][3].x )
            + (corners[i][3].x * corners[i][0].y - corners[i][3].y * corners[i][0].x )  ) / 2));

            //std::cout << ids[i] << " - " << areas[i] << "\n";
        }
        cv::aruco::drawDetectedMarkers(frame, corners, ids);
    }
    //std::cout << areas.front() << std::endl;
    //cv::flip(_canvas,_canvas,0);
    frame.copyTo(_canvas);

    cv::flip(_canvas,_canvas,0);
    areas.clear();
    cv::Mat edges;

     if (vid.isOpened()) {
        //vid_mutex.lock();
            //@@@THREADING STUFF

        server.get_cmd(cmds);
        if (cmds.size() > 0){
          for (int i = 0; i < cmds.size(); i++){
            std::cout << "\n Rx: " << cmds.at(i);
//            for(size_t i = 0; i < commandList.size()-1; i++) {
//                std::cout << myVector[i] << " ";
//                if (cmds.at(i) == commandList.at(i)){
//                    key = commandList.at(i).at(0);
//                }
//            }
            if (cmds.at(i) == "w") {
                key = 'w';
            } else if (cmds.at(i) == "s") {
                key = 's';
            } else if (cmds.at(i) == "a") {
                key = 'a';
            } else if (cmds.at(i) == "d") {
                key = 'd';
            } else if (cmds.at(i) == "x") {
                key = 'x';
            } else if (cmds.at(i) == "z") {
                key = 'z';
            } else if (cmds.at(i) == "j") {
                key = 'j';
            } else if (cmds.at(i) == "k") {
                key = 'k';
            } else if (cmds.at(i) == "l") {
                key = 'l';
            } else if (cmds.at(i) == "c") {
                key = 'c';
            } else if (cmds.at(i) == "e") {
                key = 'e';
            } else if (cmds.at(i) == "1") {
                key = '1';
            } else if (cmds.at(i) == "2") {
                key = '2';
            } else if (cmds.at(i) == "3") {
                key = '3';
            } else if (cmds.at(i) == "4") {
                key = '4';
            }

          }
        }
            //server.send_string("random word  - test response");
    }
}
void CGuidance::vidThread(CServer* server)//
{
  // Start server
  server->start(1234);

}
void CGuidance::serverThread(CServer* server)//
{
  // Start server
  server->start(4618);

}
void CGuidance::imageThread(CGuidance* ptr) {
    std::cout<< "Getting image " << ptr->isThreading;
    while (ptr->isThreading) {
        ptr->get_im();

    }
}
void CGuidance::updateThread(CGuidance* ptr) {
    std::cout<< "Getting image " << ptr->isThreading;
    while (ptr->isThreading) {
        ptr->update();

    }
}
void CGuidance::get_im(){
    if (vid.isOpened()) {

        //cv::imshow(CANVAS_NAME, _canvas);
        //drawGUI();
        if(key == 'q'){
            isThreading = false;
        }
        //vid_mutex.lock();
        server.set_txim(_canvas);
        //vid_mutex.unlock();
        std::this_thread::sleep_until(calc_start + std::chrono::milliseconds(30));
        calc_end = std::chrono::steady_clock::now();
        deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start) ;
        calc_start = std::chrono::steady_clock::now();
    }else{
        std::cout<< "Camera is not openned\n";
        //exit(0);
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

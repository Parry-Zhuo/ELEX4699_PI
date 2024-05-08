#include "CCar.h"
#include "CGuidance.h"
#include "CMotor.h"


#define _pinleftPWM 12
#define _pinrightPWM 13

#define _pinleftDIR 6
#define _pinrightDIR 16

#define CANVAS_NAME "POTATO CAMERA DISPLAY  "
CCar::CCar() {


}

CCar::~CCar() {
    // Destructor implementation
    disableMotors();
    _guidance.isThreading = false;
    cv::destroyAllWindows();
    if (_modeThread.joinable()) {
        _modeThread.join();
    }
    if (_guidanceThread.joinable()) {
        _guidanceThread.join();
    }

    exit(0);
}


void CCar::drive() {
    // Drive implementation
     //while(true){
     //q
    //}
    //ONE GET INPUT FROM CGUIDANCE
    //ok so we start from here. Getting an INPUT
    //ACT UPON ACTION
    //SEND OUTPUT. NOT NEEDED
    /*if key == 'q' ... dadada*/

    /*

    _display_im = cv::Mat::zeros(canvasSize.width/2, canvasSize.height/2, CV_8UC3);
    */
    cv::Size canvasSize = cv::Size(300, 900);
    _CGuidanceimage = cv::Mat::zeros(canvasSize.width/2, canvasSize.height/2, CV_8UC3);
    std::cout<<"Press m for MANUAL MODE" << "\n";
    std::cout<<"Press a for AUTOMATIC MODE " << "\n";

    _guidance.key == -1;
    cv::imshow("CANVAS_NAME", _guidance._canvas);
    _guidance.key = cv::waitKey(0);// sets mode
    _guidance.isThreading = true;
    _motors.enableMotor();
    //std::cout << "chosen mode " << _guidance.key << "\n"; // Output the pressed key

    if(_guidance.key == 'm'){

        _guidanceUpdateThread= std::thread(&CGuidance::updateThread, &_guidance);
        _guidanceThread= std::thread(&CGuidance::imageThread, &_guidance);
        _modeThread = std::thread(&CCar::modethrd, this, "manual");
        _vid_thread = std::thread(&CGuidance::vidThread,&_guidance.server);
        _server_thread = std::thread(&CGuidance::serverThread,&_guidance.server);

        _guidanceUpdateThread.detach();
        _modeThread.detach();
        _guidanceThread.detach();
        _server_thread.detach();
        _vid_thread.detach();
        //std::cout << "key is  " << _guidance.key << "\n"; // Output the pressed key
        while (_guidance.key != 'q') {}
        //manuelMode();
    }else if(_guidance.key == 'a'){
        _guidanceThread= std::thread(&CGuidance::imageThread, &_guidance);
        _modeThread = std::thread(&CCar::modethrd, this, "auto");
        _modeThread.join();
        _guidanceThread.join();
        while (_guidance.key != 'q') {}

        //_guidance.isThreading = false;
    }else if(_guidance.key == 't'){//test mode
        std::cout<< "test mode \n";
        while(_guidance.key != 'q'){
            //_guidance.get_im();// I think i'd have to multithread this.
            //_guidance.key = cv::waitKey(1);
            manuelMode();
        }
    }

    //  _guidance.get_im();// I think i'd have to multithread this.
    disableMotors();
    _guidance.isThreading = false;

}
void CCar::disableMotors() {
    _motors._control.set_data(1,_motors._control._pinEnLmotor,0);
    _motors._control.set_data(1,_motors._control._pinEnRmotor,0);
}
void CCar::modethrd(CCar* ptr, std::string _modeName) {

//    std::cout<< "Getting image " << ptr->isThreading;
    if(_modeName == "manual"){
        while (ptr->_guidance.isThreading) {
            ptr->manuelMode();
        }
    }else if(_modeName == "auto"){
        //while (ptr->_guidance.isThreading) {
        ptr->autoMode();
        //}
    }
}

void CCar::autoMode(){

    //_motors.forwards(1750);
    //_motors.stop();
    //_motors.shoot(1);
    _motors.forwards(1750);
    //_motors.right(800);
    int widthTime = 3500;
    auto now = std::chrono::steady_clock::now();
    while(_guidance.isShoot == false){
        _motors.forwards();
    }
    _guidance.isShoot = false;
    _motors.stop();
    _motors.shoot();

    auto _timeElapsed = std::chrono::steady_clock::now() - now;
    auto timeLeft =  _timeElapsed - std::chrono::milliseconds(widthTime) ;
    //std::cout<<"timeLeft " << timeLeft ;
    //while(finishTime >  std::chrono::system_clock::now()){}
    _motors.forwards(timeLeft.count());

    //now turn right again
    _motors.right(800);
    _motors.forwards(1750);

    _motors.stop();
    _motors.shoot();

    _motors.right(400);//45 degree turn or less
    auto now2 = std::chrono::steady_clock::now();
    while(_guidance.isShoot == false){
        _motors.forwards();
    }
    auto _timeElapsed2 = std::chrono::steady_clock::now() - now2;
    _guidance.isShoot = false;
    _motors.stop();
    _motors.shoot();

    _motors.backward(_timeElapsed2.count());
    _motors.left(400);//45 degree turn or less
    _motors.forwards(1000);

    _motors.stop();
    _motors._control.set_data(1,_motors._control._pinEnLmotor,0);
    _motors._control.set_data(1,_motors._control._pinEnRmotor,0);

}
void CCar::manuelMode(){

    //while(_guidance.key != 'q'){
    cv::imshow(CANVAS_NAME, _guidance._canvas);
    int cmd = cv::waitKey(1);
    if(cmd != -1){//for manual by hand.
        _guidance.key = cmd;
    }


    int timeDuration = 0;
    if(_guidance.key == 'w'){
        _motors.forwards(timeDuration);
    }else if(_guidance.key == 'a'){
        _motors.left(timeDuration);
    }else if(_guidance.key == 'd'){
        _motors.right(timeDuration);
    }else if(_guidance.key == 's'){
        _motors.backward(timeDuration);
    }else if(_guidance.key == 'z'){// to shoot!
        _motors.shoot(CMotor::SHOOTPOS);
    }else if(_guidance.key == 'q'){// to shoot!
        _guidance.isThreading = false;
    }else if(_guidance.key == 'x'){
        //_guidance.key == ' ';
        _motors.cry();
    }
    //std::cout<<"guidance key " <<  _guidance.key << "\n";

    //delay
    //}
}
void CCar::start(){
/*
    char inputchar = 0;

    isThreading = true;

    update_thread = std::thread(&CRecyclingSort::updateThread, this);
    draw_thread = std::thread(&CRecyclingSort::drawThread, this);
    server_thread = std::thread(&CRecyclingSort::serverThread,&server);
    vid_thread = std::thread(&CRecyclingSort::vidThread,&server);

    update_thread.join();
    draw_thread.join();

    server_thread.detach();
    vid_thread.detach();


    while (isThreading == true){
        if(inputchar == 'q'){
            isThreading = false;
        }
    }
    end();
    server.stop();
    sleep(100);//not sure why this is here but okay
*/
}
void CCar::serverthrd(CCar* ptr) {
    // Server thread handling
}

void CCar::imagethrd(CCar* ptr) {
    // Image processing thread handling
    /*
    so we will be running this thread here.
    */
    //while (!ptr->_thread_exit) {
      //  ptr->_guidance->update();
    //}
}

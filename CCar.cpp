#include "CCar.h"
#include "CGuidance.h"
#include "CMotor.h"


#define _pinleftPWM 12
#define _pinrightPWM 13

#define _pinleftDIR 6
#define _pinrightDIR 16

CCar::CCar() {


}

CCar::~CCar() {
    // Destructor implementation
    _guidance.isThreading = false;
    if (_modeThread.joinable()) {
        _modeThread.join();
    }
    if (_guidanceThread.joinable()) {
        _guidanceThread.join();
    }
    _motors._control.set_data(1,_motors._control._pinEnLmotor,0);
    _motors._control.set_data(1,_motors._control._pinEnRmotor,0);

    exit(0);
}


void CCar::drive() {
    // Drive implementation
     //while(true){
     //
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

    while(_guidance.key != 's'){
        _guidance.get_im();// I think i'd have to multithread this.
        if(_guidance.key == 'm'){
            break;
        }else if(_guidance.key == 'a'){
            break;
        }
    }
    _guidance.isThreading = true;
    _motors.enableMotor();

    //std::thread guidanceThread= std::thread(&CGuidance::imageThread, &_guidance);
    //guidanceThread.detach();
    if(_guidance.key == 'm'){
        _guidanceThread= std::thread(&CGuidance::imageThread, &_guidance);
        _modeThread = std::thread(&CCar::modethrd, this, "manual");
        _modeThread.detach();
        _guidanceThread.detach();
        while (_guidance.key != 'q') {
        }
        //manuelMode();
    }else if(_guidance.key == 'a'){
        _guidanceThread= std::thread(&CGuidance::imageThread, &_guidance);
        _modeThread = std::thread(&CCar::modethrd, this, "auto");
        _modeThread.join();
        _guidanceThread.join();
    }else if(_guidance.key == 't'){//test mode
        while(_guidance.key != 'q'){
            _guidance.get_im();// I think i'd have to multithread this.
        }
    }


    //  _guidance.get_im();// I think i'd have to multithread this.

    _guidance.isThreading = false;

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

    _motors.forwards(1750);
    _motors.stop();
    _motors.shoot();
    _motors.forwards(1750);
    _motors.right(800);

    _motors.stop();
    _motors._control.set_data(1,_motors._control._pinEnLmotor,0);
    _motors._control.set_data(1,_motors._control._pinEnRmotor,0);

}
void CCar::manuelMode(){

    //while(_guidance.key != 'q'){
        //std::cout<<" guidance key" << _guidance.key << "\n";

        int timeDuration = 50;
        if(_guidance.key == 'w'){
            _motors.forwards();
        }else if(_guidance.key == 'a'){
            _motors.left();
        }else if(_guidance.key == 'd'){
            _motors.right();
        }else if(_guidance.key == 's'){
            _motors.backward(timeDuration);
        }else if(_guidance.key == 'z'){// to shoot!
            _motors.shoot(CMotor::SHOOTPOS);
        }else{
        //if no command
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

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
    std::cout<<"Press s for MANUAL MODE" << "\n";
    std::cout<<"Press a for AUTOMATIC MODE " << "\n";

    while(_guidance.key != 's'){
        _guidance.get_im();// I think i'd have to multithread this.
    }
    _guidance.isThreading = true;
    _motors.enableMotor();

    //std::thread guidanceThread= std::thread(&CGuidance::imageThread, &_guidance);
//    guidanceThread.detach();
    if(_guidance.key == 's'){
        _guidanceThread= std::thread(&CGuidance::imageThread, &_guidance);
        _manualThread = std::thread(&CCar::manualModethrd, this);

        _manualThread.join();
        _guidanceThread.join();
        //manuelMode();
    }else if(_guidance.key == 'a'){
        //autoMode();
    }


    //_guidance.get_im();// I think i'd have to multithread this.

    _guidance.isThreading = false;

}
void CCar::manualModethrd(CCar* ptr) {

//    std::cout<< "Getting image " << ptr->isThreading;
    while (ptr->_guidance.isThreading) {
        ptr->manuelMode();
    }
}
void CCar::manuelMode(){

    //while(_guidance.key != 'q'){
        std::cout<<" guidance key" << _guidance.key << "\n";
        /*So the issue is that _guidance.key does not always occur as there's a pause.*/
        int timeDuration = 50;
        if(_guidance.key == 'w'){
            _motors.forwards();
        }else if(_guidance.key == 'd'){
            _motors.left(timeDuration);
        }else if(_guidance.key == 'a'){
            _motors.right(timeDuration);
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
/*

void PPong::update_thread(PPong* ptr) {
    std::cout<<"thread exit" << !ptr->_thread_exit << "gamePause" << !ptr->gamePause << "\n";
    while (!ptr->_thread_exit) {
        if (!ptr->gamePause) {
            ptr->update();
        }
        // No need to spawn a new thread here, just execute update
    }
}

*/
void CCar::imagethrd(CCar* ptr) {
    // Image processing thread handling
    /*
    so we will be running this thread here.
    */
    //while (!ptr->_thread_exit) {
      //  ptr->_guidance->update();
    //}
}

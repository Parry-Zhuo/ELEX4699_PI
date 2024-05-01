#include "CCar.h"
#include "CGuidance.h"
#include "CMotor.h"


#define _pinleftPWM 12
#define _pinrightPWM 13

#define _pinleftDIR 6
#define _pinrightDIR 16

CCar::CCar() {
    // Constructor implementation
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
    std::cout<<"start the program by pressing s" << "\n";

    while(_guidance.key != 's'){
        _guidance.get_im(_CGuidanceimage);// I think i'd have to multithread this.
    }
    _motors.enableMotor();
    while(_guidance.key != 'q'){

        _guidance.get_im(_CGuidanceimage);// I think i'd have to multithread this.



        std::cout<<"potato is running with key " << _guidance.key << "\n";
        int timeDuration = 50;
        if(_guidance.key == 'w'){
            _motors.forwards(timeDuration);
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

            _motors.cry(timeDuration);
        }
        std::cout<<"guidance key " <<  _guidance.key << "\n";
        _guidance.key == ' ';
        //delay


    }

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

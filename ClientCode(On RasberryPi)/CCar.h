
#pragma once
#include "server.h"
#include "CMotor.h"
#include "CGuidance.h"
#include "CControler.h"

#include <opencv2/highgui.hpp>
#include <vector>
#include <thread>

class CCar
{
private:

	CServer _server;
	CMotor _motors;
	CGuidance _guidance;
	static void serverthrd(CCar* ptr);
	static void imagethrd(CCar* ptr);
	static void modethrd(CCar* ptr,std::string _modeName);
    void drawGUI();
    std::thread _guidanceThread;
    std::thread _modeThread;
    std::thread _guidanceUpdateThread;
    std::thread _server_thread; ///< Thread for handling camera.
    std::thread _vid_thread;
    bool _isThreading;

    cv::Mat _CGuidanceimage; // Declare a variable to hold the captured image

    /*Different modes*/
    void manuelMode();
    void autoMode();
    /*THREAD and SERVER STUFF*/
    CServer server;
    CServer _outputserver;

    static void serverThread(CServer* server);//, int port
    static void CGuidanceThread(CGuidance* camera);//, int port

    void disableMotors();
    void cameraDistance(int id, int distance, std::string operation);
    //static void updateThread(CRecyclingSort* ptr);
    //static void drawThread(CRecyclingSort* ptr);

public:
    int mode;
	CCar();
	~CCar();
	void drive();
    void start();
};

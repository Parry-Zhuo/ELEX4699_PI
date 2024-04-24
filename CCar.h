
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
public:
	CCar();
	~CCar();
	void drive();

};

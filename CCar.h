#include "Server.h"
#include "Motor.h"
#include "Guidance.h"
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
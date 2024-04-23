#include "server.h"
#include "CMotor.h"
#include "CGuidance.h"
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

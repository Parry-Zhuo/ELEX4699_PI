#include "CCar.h"
#include "CGuidance.h"
#include "CMotor.h"

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
    _motors.forwards(20000);
    std::cout<<"potato is running\n";
}

void CCar::serverthrd(CCar* ptr) {
    // Server thread handling
}

void CCar::imagethrd(CCar* ptr) {
    // Image processing thread handling
}

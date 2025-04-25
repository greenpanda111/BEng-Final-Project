#ifndef Movement_h
#define Movement_h

#include "mbed.h"

using namespace mbed;

#define RIGHT_MOTOR_PWM P0_1
#define RIGHT_MOTOR_DIRECTION P0_4
#define LEFT_MOTOR_PWM P1_8
#define LEFT_MOTOR_DIRECTION P0_5
#define PWM_PERIOD 0.1f

class Motor
{
public:
    Motor(PinName pwmPin, PinName dirPin, bool forwardDirection);
    void move(float power);
    void stop();
    void setup();

private:
    PwmOut _pwmPin;
    DigitalOut _dirPin;
    bool _forwardDirection;

};

extern Motor leftMotor;
extern Motor rightMotor;

#endif // MOVEMENT_H
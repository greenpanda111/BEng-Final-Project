#include "Movement.h"
#include "mbed.h"

using namespace mbed;

Motor::Motor(PinName pwmPin, PinName dirPin, bool forwardDirection) : _pwmPin(pwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection)
{
}

void Motor::move(float power)
{
    if (power > 0)
    {
        _dirPin.write(_forwardDirection);
    }
    else
    {
        _dirPin.write(!_forwardDirection);
    }
    _pwmPin.write(abs(power));
}

void Motor::stop()
{
    _pwmPin.write(0.0f);
}

void Motor::setup()
{
    _pwmPin.period(PWM_PERIOD);
    _pwmPin.write(0.0f);
}

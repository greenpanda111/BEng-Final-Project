#include "Arduino.h"
#include "Movement.h"

#define DEFAULT_SPEED 1
#define LEFT_FORWARD 1
#define RIGHT_FORWARD 0

unsigned char opcode;

Motor leftMotor(LEFT_MOTOR_PWM, LEFT_MOTOR_DIRECTION, LEFT_FORWARD);
Motor rightMotor(RIGHT_MOTOR_PWM, RIGHT_MOTOR_DIRECTION, RIGHT_FORWARD);

void forward();
void reverse();
void stop();
void turnLeft();
void turnRight();

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    leftMotor.setup();
    rightMotor.setup();
    Serial1.begin(115200);
}

void loop()
{
    if (Serial1.available() > 0)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        opcode = Serial1.read();
        Serial.println(opcode);

        if ((char)opcode == 'F')
        {
            forward();
            Serial.println("Forwards");
            delay(500);
            stop();
        }
        if ((char)opcode == 'B')
        {
            reverse();
            Serial.println("Backwards");
            delay(500);
            stop();
        }
        if ((char)opcode == 'L')
        {
            turnLeft();
            Serial.println("Left");
            delay(500);
            stop();
        }
        if ((char)opcode == 'R')
        {
            turnRight();
            Serial.println("Right");
            delay(500);
            stop();
        }
        if ((char)opcode == 's')
        {
            stop();
        }
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void forward()
{
    leftMotor.move(DEFAULT_SPEED);
    rightMotor.move(DEFAULT_SPEED);
}

void reverse()
{
    leftMotor.move(-DEFAULT_SPEED);
    rightMotor.move(-DEFAULT_SPEED);
}

void stop()
{
    leftMotor.move(0.0f);
    rightMotor.move(0.0f);
}

void turnLeft()
{
    leftMotor.move(DEFAULT_SPEED);
    rightMotor.move(-DEFAULT_SPEED);
}

void turnRight()
{
    leftMotor.move(-DEFAULT_SPEED);
    rightMotor.move(DEFAULT_SPEED);
}

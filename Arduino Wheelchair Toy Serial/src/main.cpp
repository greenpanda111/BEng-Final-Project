#include "Arduino.h"
#include "Movement.h"

#define DEFAULT_SPEED 0.8
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

int mode = 'Z';
int X = 0;
int Y = 0;
bool X_Flag = false;
bool Y_Flag = false;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    leftMotor.setup();
    rightMotor.setup();
    Serial.begin(115200);
}

void loop()
{
    if (Serial.available() > 0)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        opcode = Serial.read();

        if ((char)opcode == 'Z' || (char)opcode == 'X' || (char)opcode == 'C' || (char)opcode == 'V')
        {
            mode = (char)opcode;
        }

        char command = (char)opcode;

        switch (mode)
        {
        case ('Z'):
            if (command == 'F')
            {
                forward();
                delay(50);
                stop();
            }
            if (command == 'B')
            {
                reverse();
                delay(50);
                stop();
            }
            if (command == 'L')
            {
                turnLeft();
                delay(50);
                stop();
            }
            if (command == 'R')
            {
                turnRight();
                delay(50);
                stop();
            }
            break;

        case ('N'):
            if (command == 'F')
            {
                forward();
                delay(50);
                stop();
            }
            if (command == 'O')
            {
                leftMotor.move(DEFAULT_SPEED);
                delay(50);
                stop();
            }
            if (command == 'P')
            {
                rightMotor.move(DEFAULT_SPEED);
                delay(50);
                stop();
            }

            break;

        case ('C'):
            turnRight();
            if (command == 'F')
            {
                stop();
                forward();
                delay(50);
                stop();
            }
            break;

        case ('V'):
            if (X_Flag == true)
            {
                X = opcode;
                X_Flag = false;
                Y_Flag = true;
            }
            else if (Y_Flag == true)
            {
                Y = opcode;
                Y_Flag = false;
            }
            if (command == 'X')
            {
                X_Flag = true;
            }
            if ((X > 0) & (Y > 0))
            {
                // Map joystick values from (0-1023) to (-512 to +512)
                int mappedX = map(X, 0, 1023, -512, 512);
                int mappedY = map(Y, 0, 1023, -512, 512);

                // Mix for differential drive
                int leftSpeed = mappedY + mappedX;
                int rightSpeed = mappedY - mappedX;

                // Constrain raw speeds
                leftSpeed = constrain(leftSpeed, -512, 512);
                rightSpeed = constrain(rightSpeed, -512, 512);

                // Normalize speeds to 0.0 - 1.0 range
                float leftPWM = (float)(leftSpeed + 512) / 1024.0;   // Map from [-512, 512] -> [0.0, 1.0]
                float rightPWM = (float)(rightSpeed + 512) / 1024.0; // Same mapping

                // Constrain just in case (floating-point precision safety)
                leftPWM = constrain(leftPWM, 0.0, 1.0);
                rightPWM = constrain(rightPWM, 0.0, 1.0);

                leftMotor.move(leftPWM);
                rightMotor.move(rightPWM);
            }
            break;
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

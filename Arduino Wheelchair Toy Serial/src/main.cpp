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

int mode;
int X;
int Y;
bool X_Flag;
bool Y_Flag;
char command;
int num;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    leftMotor.setup();
    rightMotor.setup();
    Serial.begin(115200);
    mode = 'Z';
    num = 0;
    X = -1;
    Y = -1;
    X_Flag = false;
    Y_Flag = false;
    command = 'S';
}

void loop()
{
    if (Serial.available() > 0)
    {
        if (Serial.available() > 1)
        {
            char header = Serial.read();
            if (header == 'I')
            {
                uint8_t b0 = Serial.read();
                uint8_t b1 = Serial.read();
                uint8_t b2 = Serial.read();
                uint8_t b3 = Serial.read();

                num = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
                if (X_Flag == true)
                {
                    X = num;
                    X_Flag = false;
                }
                else if (Y_Flag == true)
                {
                    Y = num;
                    Y_Flag = false;
                }
            }
            else if (header == 'C')
            {
                // Wait for 1 character byte
                if (Serial.available() >= 1)
                {
                    if (opcode == 'X')
                    {
                        X_Flag = true;
                    }
                    else if (opcode == 'Y')
                    {
                        Y_Flag = false;
                    }
                    else
                    {
                        opcode = (char)Serial.read();
                    }
                }
            }
        }

        if (opcode == 'Z' || opcode == 'X' || opcode == 'C' || opcode == 'V')
        {
            mode = opcode;
        }
        else
        {
            command = opcode;
        }

        switch (mode)
        {
        case ('Z'):
            if (command == 'F')
            {
                forward();
                delay(10);
                stop();
            }
            if (command == 'B')
            {
                reverse();
                delay(10);
                stop();
            }
            if (command == 'L')
            {
                turnLeft();
                delay(10);
                stop();
            }
            if (command == 'R')
            {
                turnRight();
                delay(10);
                stop();
            }
            break;

        case ('N'):
            digitalWrite(LED_BUILTIN, HIGH);
            if (command == 'F')
            {
                forward();
                delay(10);
                stop();
            }
            if (command == 'O')
            {
                leftMotor.move(DEFAULT_SPEED);
                delay(10);
                stop();
            }
            if (command == 'P')
            {
                rightMotor.move(DEFAULT_SPEED);
                delay(10);
                stop();
            }

            break;

        case ('C'):
            turnRight();
            if (command == 'F')
            {
                stop();
                forward();
                delay(100);
                stop();
            }
            break;

        case ('V'):

            if ((X >= 0) & (Y >= 0))
            {

                // Map joystick values
                int mappedX = map(X, 0, 4095, -2048, 2048);
                int mappedY = map(Y, 0, 4095, -2048, 2048);

                // Mix for differential drive
                int leftSpeed = mappedY + mappedX;
                int rightSpeed = mappedY - mappedX;

                // Constrain raw speeds
                leftSpeed = constrain(leftSpeed, -2048, 2047);
                rightSpeed = constrain(rightSpeed, -512, 2047);

                // Normalize speeds to 0.0 - 1.0 range
                float leftPWM = (float)(leftSpeed + 2048) / 4095.0; // Map from [-2048, 2048] -> [0.0, 1.0]
                float rightPWM = (float)(rightSpeed + 2048) / 4095.0;

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
    delay(10);
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

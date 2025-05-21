#include "Arduino.h"
#include "Movement.h"

#define DEFAULT_SPEED 0.8
#define LEFT_FORWARD 1
#define RIGHT_FORWARD 0

Motor leftMotor(LEFT_MOTOR_PWM, LEFT_MOTOR_DIRECTION, LEFT_FORWARD);
Motor rightMotor(RIGHT_MOTOR_PWM, RIGHT_MOTOR_DIRECTION, RIGHT_FORWARD);

void forward();
void reverse();
void stop();
void turnLeft();
void turnRight();

char mode;
int X;
int Y;
bool X_Flag;
bool Y_Flag;
char command;
int num;
char opcode;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    leftMotor.setup();
    rightMotor.setup();
    Serial1.begin(115200);
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
    if (Serial1.available() > 0)
    {
        if (Serial1.available() > 1)
        {
            char header = Serial1.read();
            Serial.print(header);
            Serial.println(" received");

            if (header == 'I')
            {
                uint8_t b0 = Serial1.read();
                uint8_t b1 = Serial1.read();
                uint8_t b2 = Serial1.read();
                uint8_t b3 = Serial1.read();

                num = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
                if (X_Flag == true)
                {
                    X = num;
                    X_Flag = false;
                    Serial.print("x:");
                    Serial.println(X);
                }
                else if (Y_Flag == true)
                {
                    Y = num;
                    Y_Flag = false;
                    Serial.print("y:");
                    Serial.println(Y);
                }
            }
            else if (header == 'C')
            {
                // Wait for 1 character byte
                if (Serial1.available() >= 1)
                {
                    opcode = (char)Serial1.read();
                    Serial.print(opcode);
                    Serial.println(" command received");

                    if (opcode == 'X')
                    {
                        X_Flag = true;
                        Serial.println("x received");
                    }
                    else if (opcode == 'Y')
                    {
                        Y_Flag = true;
                        Serial.println("y received");
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
                Serial.println("4 forward");
                forward();
                delay(10);
                stop();
            }
            if (command == 'B')
            {
                Serial.println("4 backward");
                reverse();
                delay(10);
                stop();
            }
            if (command == 'L')
            {
                Serial.println("4 left");
                turnLeft();
                delay(10);
                stop();
            }
            if (command == 'R')
            {
                Serial.println("4 right");
                turnRight();
                delay(10);
                stop();
            }
            break;

        case ('N'):
            digitalWrite(LED_BUILTIN, HIGH);
            if (command == 'F')
            {
                Serial.println("2 forward");
                forward();
                delay(100);
                stop();
            }
            if (command == 'O')
            {
                Serial.println("2 left forward");
                leftMotor.move(DEFAULT_SPEED);
                delay(100);
                stop();
            }
            if (command == 'P')
            {
                Serial.println("2 right forward");
                rightMotor.move(DEFAULT_SPEED);
                delay(100);
                stop();
            }

            break;

        case ('C'):
            turnRight();
            delay(100);
            stop();
            if (command == 'F')
            {
                Serial.println("1 forward");
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
                int mappedX = map(X, 0, 4096, -2048, 2047);
                int mappedY = map(Y, 0, 4096, -2048, 2047);

                Serial.print("mapX:");
                Serial.println(mappedX);

                if (abs(X) < 100)
                    X = 0;
                if (abs(Y) < 100)
                    Y = 0;

                // Mix for differential drive
                int leftSpeed = mappedY + mappedX;
                int rightSpeed = mappedY - mappedX;

                // Constrain raw speeds
                leftSpeed = constrain(leftSpeed, -2048, 2048);
                rightSpeed = constrain(rightSpeed, -2048, 2048);

                float leftPWM = constrain(leftPWM, -1.0, 1.0);
                float rightPWM = constrain(rightPWM, -1.0, 1.0);

                Serial.print("leftPWM:");
                Serial.println(leftPWM);

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
    Serial.println("forward");
    leftMotor.move(DEFAULT_SPEED);
    rightMotor.move(DEFAULT_SPEED);
}

void reverse()
{
    Serial.println("reverse");

    leftMotor.move(-DEFAULT_SPEED);
    rightMotor.move(-DEFAULT_SPEED);
}

void stop()
{
    Serial.println("stop");

    leftMotor.move(0.0f);
    rightMotor.move(0.0f);
}

void turnLeft()
{
    Serial.println("left");

    leftMotor.move(DEFAULT_SPEED);
    rightMotor.move(-DEFAULT_SPEED);
}

void turnRight()
{
    Serial.println("right");

    leftMotor.move(-DEFAULT_SPEED);
    rightMotor.move(DEFAULT_SPEED);
}

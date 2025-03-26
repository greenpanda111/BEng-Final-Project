#include <Arduino.h>
#include "mbed.h"
#include "Movement.h"

#define DEFAULT_SPEED 1
#define LEFT_FORWARD 1
#define RIGHT_FORWARD 0

using namespace mbed;

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
  Serial1.begin(9700);
  Serial.begin(9600);
  leftMotor.setup();
  rightMotor.setup();
}

void loop()
{
  Serial.println((char)Serial1.read());

  //if (Serial1.available())
  //{
    opcode = Serial1.read();
    if ((char)opcode == 'f')
    {
      forward();
      Serial.println("Forwards");
      // delay(10);
      // stop();
    }
    if ((char)opcode == 'b')
    {
      reverse();
      Serial.println("Backwards");
      // delay(10);
      // stop();
    }
    if ((char)opcode == 'l')
    {
      turnLeft();
      Serial.println("Left");
      // delay(10);
      // stop();
    }
    if ((char)opcode == 'r')
    {
      turnRight();
      Serial.println("Right");
      // delay(10);
      // stop();
    }
    if ((char)opcode == 's')
    {
      stop();
    }
 // }
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
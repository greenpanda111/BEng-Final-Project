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
      char command = Serial1.read();
      Serial.print(command);
      Serial.println(" received");

      if (command == 'F')
      {
        Serial.println("forward");
        forward();
      }
      if (command == 'B')
      {
        Serial.println("backward");
        reverse();
      }
      if (command == 'L')
      {
        Serial.println("left");
        turnLeft();
      }
      if (command == 'R')
      {
        Serial.println("right");
        turnRight();
      }
      else
      {
        Serial.println("Stopping");
        stop();
      }
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
    delay(10);
  }
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

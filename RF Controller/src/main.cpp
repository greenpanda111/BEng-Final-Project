#include <Arduino.h>

// variables for button
#define forwardsButtonPin A0
int oldForwardsButtonState = LOW;
#define backwardsButtonPin A1
int oldBackwardsButtonState = LOW;
#define leftButtonPin A2
int oldLeftButtonState = LOW;
#define rightButtonPin A3
int oldRightButtonState = LOW;

#define xPin A6
#define yPin A7
#define LEFT_THRESHOLD 400
#define RIGHT_THRESHOLD 800
#define FORWARDS_THRESHOLD 400
#define BACKWARDS_THRESHOLD 800

int xValue;
int yValue;

void controlMotors();

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  int xValue = 0;
  int yValue = 0;

  // configure the buttons pin as input
  pinMode(forwardsButtonPin, INPUT);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
}

void loop()
{
  controlMotors();
}

void controlMotors()
{
  // read joystick pins
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  if (yValue < FORWARDS_THRESHOLD)
  {
    Serial1.write((byte)'f');
    Serial.println("joystick forwards");
  }
  else if (yValue > BACKWARDS_THRESHOLD)
  {
    Serial1.write((byte)'b');
    Serial.println("joystick backwards");
  }

  if (xValue < LEFT_THRESHOLD)
  {
    Serial1.write((byte)'l');
    Serial.println("joystick left");
  }
  else if (xValue > RIGHT_THRESHOLD)
  {
    Serial1.write((byte)'l');
    Serial.println("joystick right");
  }

  // read the button pins
  int forwardsButtonState = digitalRead(forwardsButtonPin);
  int backwardsButtonState = digitalRead(backwardsButtonPin);
  int leftButtonState = digitalRead(leftButtonPin);
  int rightButtonState = digitalRead(rightButtonPin);

  if (oldForwardsButtonState != forwardsButtonState)
  {
    // button changed
    oldForwardsButtonState = forwardsButtonState;

    if (forwardsButtonState)
    {
      Serial.println("Forwards Button pressed");

      // forwards button is pressed, write forwards
      Serial1.write((byte)'f');
    }
    else
    {
      Serial.println("button released");

      // button is released, write stop
      Serial1.write((byte)'s');
    }
  }

  if (oldBackwardsButtonState != backwardsButtonState)
  {
    // button changed
    oldBackwardsButtonState = backwardsButtonState;

    if (backwardsButtonState)
    {
      Serial.println("Backwards Button pressed");

      // backwards button is pressed, write forwards
      Serial1.write((byte)'b');
    }
    else
    {
      Serial.println("button released");

      // button is released, write stop
      Serial1.write((byte)'s');
    }
  }

  if (oldLeftButtonState != leftButtonState)
  {
    // button changed
    oldLeftButtonState = leftButtonState;

    if (leftButtonState)
    {
      Serial.println("Left Button pressed");

      // left button is pressed, write forwards
      Serial1.write((byte)'l');
    }
    else
    {
      Serial.println("button released");

      // button is released, write stop
      Serial1.write((byte)'s');
    }
  }

  if (oldRightButtonState != rightButtonState)
  {
    // button changed
    oldRightButtonState = rightButtonState;

    if (rightButtonState)
    {
      Serial.println("Right Button pressed");

      // right button is pressed, write forwards
      Serial1.write((byte)'r');
    }
    else
    {
      Serial.println("button released");

      // button is released, write stop
      Serial1.write((byte)'s');
    }
  }
}
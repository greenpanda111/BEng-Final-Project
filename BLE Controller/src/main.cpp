#include <ArduinoBLE.h>

// variables for button
#define forwardsButtonPin A0
int oldForwardsButtonState = LOW;
#define backwardsButtonPin A1
int oldBackwardsButtonState = LOW;
#define leftButtonPin A2
int oldLeftButtonState = LOW;
#define rightButtonPin A3
int oldRightButtonState = LOW;

void controlMotors(BLEDevice);

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  // configure the buttons pin as input
  pinMode(forwardsButtonPin, INPUT);

  //attachInterrupt(digitalPinToInterrupt(forwardsButtonPin), forwardsISR, RISING);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("Bluetooth® Low Energy Central - Motor control");

  // start scanning for peripherals
  BLE.scanForUuid("1234");
}

void loop()
{
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral)
  {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "Motor")
    {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlMotors(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("1234");
  }
}

void controlMotors(BLEDevice peripheral)
{
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect())
  {
    Serial.println("Connected");
  }
  else
  {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes())
  {
    Serial.println("Attributes discovered");
  }
  else
  {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the motor characteristic
  BLECharacteristic motorCharacteristic = peripheral.characteristic("1234");

  if (!motorCharacteristic)
  {
    Serial.println("Peripheral does not have motor characteristic!");
    peripheral.disconnect();
    return;
  }
  else if (!motorCharacteristic.canWrite())
  {
    Serial.println("Peripheral does not have a writable motor characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected())
  {
    // while the peripheral is connected

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
        motorCharacteristic.writeValue((byte)'f');
      }
      else
      {
        Serial.println("button released");

        // button is released, write stop
        motorCharacteristic.writeValue((byte)'s');
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
        motorCharacteristic.writeValue((byte)'b');
      }
      else
      {
        Serial.println("button released");

        // button is released, write stop
        motorCharacteristic.writeValue((byte)'s');
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
        motorCharacteristic.writeValue((byte)'l');
      }
      else
      {
        Serial.println("button released");

        // button is released, write stop
        motorCharacteristic.writeValue((byte)'s');
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
        motorCharacteristic.writeValue((byte)'r');
      }
      else
      {
        Serial.println("button released");

        // button is released, write stop
        motorCharacteristic.writeValue((byte)'s');
      }
    }
  }

  Serial.println("Peripheral disconnected");
}


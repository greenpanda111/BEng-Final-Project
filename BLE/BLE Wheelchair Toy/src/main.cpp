#include <Arduino.h>
#include <ArduinoBLE.h>
#include "mbed.h"
#include "Movement.h"

#define DEFAULT_SPEED 1
#define LEFT_FORWARD 1
#define RIGHT_FORWARD 0

using namespace mbed;

unsigned char opcode;

Motor leftMotor(LEFT_MOTOR_PWM, LEFT_MOTOR_DIRECTION, LEFT_FORWARD);
Motor rightMotor(RIGHT_MOTOR_PWM, RIGHT_MOTOR_DIRECTION, RIGHT_FORWARD);

BLEService motorService("1234"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic motorCharacteristic("1234", BLERead | BLEWrite);

void forward();
void reverse();
void stop();
void turnLeft();
void turnRight();

void setup()
{
    Serial1.begin(115200);
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    leftMotor.setup();
    rightMotor.setup();

    // begin initialization
    if (!BLE.begin())
    {
        Serial.println("starting Bluetooth® Low Energy module failed!");

        while (1)
            ;
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("Motor");
    BLE.setAdvertisedService(motorService);

    // add the characteristic to the service
    motorService.addCharacteristic(motorCharacteristic);

    // add service
    BLE.addService(motorService);

    // set the initial value for the characeristic:
    motorCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();
}

void loop()
{
    // listen for Bluetooth® Low Energy peripherals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central)
    {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());

        // while the central is still connected to peripheral:
        while (central.connected())
        {
            // if the remote device wrote to the characteristic,
            // use the value to control the LED:
            if (motorCharacteristic.written())
            {
                opcode = motorCharacteristic.value();

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
            }
        }

        // when the central disconnects, print it out:
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
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

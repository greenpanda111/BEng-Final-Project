#include <WiFi.h>

// WiFi credentials
const char *ssid = "wheelchair";
const char *password = "wheelchair123";

// Button pin macros
#define BUTTON_ONE 13
#define BUTTON_TWO 12
#define BUTTON_THREE 14
#define BUTTON_FOUR 27
#define FOUR_SWITCH_TOGGLE 26
#define TWO_SWITCH_TOGGLE 25
#define SINGLE_SWITCH_TOGGLE 33
#define JOYSTICK_TOGGLE 32
#define JOYSTICK_X 35
#define JOYSTICK_Y 34

// Server IP and port
IPAddress serverIP(10, 42, 0, 1);
const uint16_t serverPort = 5001;

WiFiClient client;

int mode = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_ONE, INPUT_PULLUP);
  pinMode(BUTTON_TWO, INPUT_PULLUP);
  pinMode(BUTTON_THREE, INPUT_PULLUP);
  pinMode(BUTTON_FOUR, INPUT_PULLUP);
  pinMode(FOUR_SWITCH_TOGGLE, INPUT_PULLDOWN);
  pinMode(TWO_SWITCH_TOGGLE, INPUT_PULLDOWN);
  pinMode(SINGLE_SWITCH_TOGGLE, INPUT_PULLDOWN);
  pinMode(JOYSTICK_TOGGLE, INPUT_PULLDOWN);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (!client.connected())
  {
    if (!client.connect(serverIP, serverPort))
    {
      Serial.println("Connection to host failed");
      delay(1000);
      return;
    }
    Serial.println("Connected to server");
  }

  if (digitalRead(FOUR_SWITCH_TOGGLE) == HIGH)
  {
    mode = 0;
  }
  else if (digitalRead(TWO_SWITCH_TOGGLE == HIGH))
  {
    mode = 1;
  }
  else if (digitalRead(SINGLE_SWITCH_TOGGLE == HIGH))
  {
    mode = 2;
  }
  else if (digitalRead(JOYSTICK_TOGGLE == HIGH))
  {
    mode = 3;
  }

  switch (mode)
  {
  case 0:
    client.write("0");
    if (digitalRead(BUTTON_ONE) == LOW)
    {
      client.write('F');
      Serial.println("Sent: F");
    }

    if (digitalRead(BUTTON_TWO) == LOW)
    {
      client.write('B');
      Serial.println("Sent: B");
    }

    if (digitalRead(BUTTON_THREE) == LOW)
    {
      client.write('L');
      Serial.println("Sent: L");
    }

    if (digitalRead(BUTTON_FOUR) == LOW)
    {
      client.write('R');
      Serial.println("Sent: R");
    }

    else
    {
      client.write('s');
    }
    break;

  case 1:
    client.write("1");
    if (digitalRead(BUTTON_ONE) == LOW & digitalRead(BUTTON_TWO) == LOW)
    {
      client.write('F');
      Serial.println("Sent: F");
    }
    else if (digitalRead(BUTTON_ONE) == LOW)
    {
      client.write("LF");
      Serial.println("Sent: LF");
    }
    else if (digitalRead(BUTTON_TWO) == LOW)
    {
      client.write("RF");
      Serial.println("Sent: RF");
    }
    break;

  case 2:
    client.write("2");
    if (digitalRead(BUTTON_ONE) == LOW)
    {
      client.write('F');
    }

  case 3:
    client.write("3");
    client.write(analogRead(JOYSTICK_X));
    client.write(analogRead(JOYSTICK_Y));

    break;
  }

  delay(10); // debounce and CPU relaxation
}
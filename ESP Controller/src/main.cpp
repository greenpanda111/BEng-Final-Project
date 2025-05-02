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

char mode = 'V';

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_ONE, INPUT_PULLUP);
  pinMode(BUTTON_TWO, INPUT_PULLUP);
  pinMode(BUTTON_THREE, INPUT_PULLUP);
  pinMode(BUTTON_FOUR, INPUT_PULLUP);
  pinMode(FOUR_SWITCH_TOGGLE, INPUT_PULLUP);
  pinMode(TWO_SWITCH_TOGGLE, INPUT_PULLUP);
  pinMode(SINGLE_SWITCH_TOGGLE, INPUT_PULLUP);
  pinMode(JOYSTICK_TOGGLE, INPUT_PULLUP);
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

void sendChar(char ch)
{
  client.write('C');
  client.write(ch);
}

void sendInt(int i)
{
  client.write('I');
  client.write((uint8_t *)&i, 4);
}

void loop()
{
  if (digitalRead(FOUR_SWITCH_TOGGLE) == LOW)
  {
    mode = 'Z';
    Serial.println("4 pin mode selected");
    client.write('Z');
  }
  else if (digitalRead(TWO_SWITCH_TOGGLE) == LOW)
  {
    mode = 'N';
    Serial.println("2 pin mode selected");
    client.write('N');
  }
  else if (digitalRead(SINGLE_SWITCH_TOGGLE) == LOW)
  {
    mode = 'C';
    Serial.println("1 pin mode selected");
    client.write('C');
  }
  else if (digitalRead(JOYSTICK_TOGGLE) == LOW)
  {
    mode = 'V';
    Serial.println("Joystick mode selected");
    client.write('V');
  }
  else
  {
    mode = 'K';
    Serial.print("No mode selected");
  }

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

  switch (mode)
  {
  case 'Z':
  {
    Serial.println("Sent 'Z' ");
    sendChar('Z');

    if (digitalRead(BUTTON_ONE) == LOW)
    {
      sendChar('F');
      Serial.println("Sent: F");
    }

    if (digitalRead(BUTTON_TWO) == LOW)
    {
      sendChar('B');
      Serial.println("Sent: B");
    }

    if (digitalRead(BUTTON_THREE) == LOW)
    {
      sendChar('L');
      Serial.println("Sent: L");
    }

    if (digitalRead(BUTTON_FOUR) == LOW)
    {
      sendChar('R');
      Serial.println("Sent: R");
    }
    break;
  }
  case 'N':
{
    Serial.println("Sent 'N' ");
    sendChar('N');
    if (digitalRead(BUTTON_ONE) == LOW & digitalRead(BUTTON_TWO) == LOW)
    {
      sendChar('F');
      Serial.println("Sent: F");
    }
    else if (digitalRead(BUTTON_ONE) == LOW)
    {
      sendChar('O');
      Serial.println("Sent: O");
    }
    else if (digitalRead(BUTTON_TWO) == LOW)
    {
      sendChar('P');
      Serial.println("Sent: P");
    }
    break;
  }
  case 'C':
  {
    Serial.println("Sent 'C' ");
    sendChar('C');

    if (digitalRead(BUTTON_ONE) == LOW)
    {
      sendChar('F');
      Serial.println("Sent: F");
    }
    break;
  }
  case 'V':
  {
    Serial.println("Sent 'V' ");
    sendChar('V');
    int x = analogRead(JOYSTICK_X);
    int y = analogRead(JOYSTICK_Y);
    Serial.println("Sent 'X' ");
    sendChar('X');
    Serial.print("Sent: ");
    Serial.println(x);
    Serial.print("Sent: ");
    sendInt(x);

    Serial.println("Sent 'Y' ");
    Serial.println(y);
    sendChar('Y');
    sendInt(y);
    break;
  }
  case 'K':
  {
    Serial.println("no mode selected");
    break;
  }
  }

  delay(1000); // debounce and CPU relaxation
}
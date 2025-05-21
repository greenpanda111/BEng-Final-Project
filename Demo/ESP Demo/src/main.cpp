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
  client.write(ch);
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
  else{
    sendChar('S');
    Serial.println("Sent: 'S'");
  }

  delay(100);
}
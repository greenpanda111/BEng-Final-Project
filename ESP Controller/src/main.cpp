#include <WiFi.h>

// WiFi credentials
const char* ssid = "wheelchair";
const char* password = "wheelchair123";

// Button pin macros
#define BUTTON_FORWARD_PIN   13
#define BUTTON_BACKWARD_PIN  12
#define BUTTON_LEFT_PIN      14
#define BUTTON_RIGHT_PIN     27

// Server IP and port
IPAddress serverIP(10, 42, 0, 1);
const uint16_t serverPort = 5001;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_FORWARD_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACKWARD_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    if (!client.connect(serverIP, serverPort)) {
      Serial.println("Connection to host failed");
      delay(1000);
      return;
    }
    Serial.println("Connected to server");
  }

  if (digitalRead(BUTTON_FORWARD_PIN) == LOW) {
    client.write('F');
    Serial.println("Sent: F");
    delay(200);
  }

  if (digitalRead(BUTTON_BACKWARD_PIN) == LOW) {
    client.write('B');
    Serial.println("Sent: B");
    delay(200);
  }

  if (digitalRead(BUTTON_LEFT_PIN) == LOW) {
    client.write('L');
    Serial.println("Sent: L");
    delay(200);
  }

  if (digitalRead(BUTTON_RIGHT_PIN) == LOW) {
    client.write('R');
    Serial.println("Sent: R");
    delay(200);
  }

  delay(10); // debounce and CPU relaxation
}
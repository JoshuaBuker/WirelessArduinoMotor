#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";

const int enB = 3;
const int in3 = 1;
const int in4 = 2;

double parseDataX(String data) {
  data.remove(data.indexOf('Y'));
  data.remove(data.indexOf('X'), 1);

  return data.toDouble();
}

double parseDataY(String data) {
  data.remove(0, data.indexOf('Y') + 1);
  data.remove(data.indexOf('Z'));

  return data.toDouble();
}

double parseDataZ(String data) {
  data.remove(0, data.indexOf('Z') + 1);

  return data.toDouble();
}

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      double x = parseDataX((char*)payload);
      double y = parseDataY((char*)payload);
      double z = parseDataZ((char*)payload);

      if (y > 0) {
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);

        analogWrite(enB, (int)(abs(y) * 255));
        Serial.println((int)(abs(y) * 255));
      } else {
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);

        analogWrite(enB, (int)(abs(y) * 255));
        Serial.println((int)(abs(y) * 255));
      }
      
      break;
  }
}

void setup() {
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(115200);
  delay(10);

  // Connect to WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Setup WebSocket event handler
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("WebSocket server started");

  // Set initial rotation direction
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void loop() {
  webSocket.loop();
}

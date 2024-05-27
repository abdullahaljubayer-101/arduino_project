#include "WiFi.h"
#include <ESP32Servo.h>

#define servoPin 12
#define ledPin 13

int servoAngle = 0;

Servo servo;

const char* ssid = "PSVM";
const char* password = "oh9Zm7n2LqCRNBTFkJHIks6O26jjXN1aLUYXk1HfvbJIJKvGip";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  servo.attach(servoPin);
  servo.write(servoAngle);
  pinMode(ledPin, OUTPUT);

  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<a href=\"/ledOn\">ON </a>");
            client.print("<a href=\"/ledOff\"> OFF</a>");

            client.print("<br><br><br>");

            client.print("<a href=\"/servoLeft\">LEFT </a>");
            client.print("<a href=\"/servoRight\"> RIGHT</a>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /ledOn")) {
          digitalWrite(ledPin, HIGH);
        }
        if (currentLine.endsWith("GET /ledOff")) {
          digitalWrite(ledPin, LOW);
        }

        if (currentLine.endsWith("GET /servoLeft")) {
          if (servoAngle < 180) {
            servoAngle += 10;
            servo.write(servoAngle);
          }
        }
        if (currentLine.endsWith("GET /servoRight")) {
          if (servoAngle > 0) {
            servoAngle -= 10;
            servo.write(servoAngle);
          }
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }
}

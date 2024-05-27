#include "WiFi.h"
#include <ESP32Servo.h>

#define servoGaragePin 15

Servo servoGarage;

const char* ssid = "TeamParadox";
const char* password = "12345678";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  servoGarage.attach(servoGaragePin);
  servoGarage.write(0);

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

            client.print("<h1>Garage Door</h1>");
            client.print("<a href=\"/open\">Open </a>");
            client.print("<a href=\"/close\"> Close</a>");

            client.print("<br><br><br>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /open")) {
          for (int i = 0; i <= 180; i += 5) {
            servoGarage.write(i);
            delay(250);
          }
        }
        if (currentLine.endsWith("GET /close")) {
          for (int i = 180; i >= 0; i -= 5) {
            servoGarage.write(i);
            delay(250);
          }
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }
}

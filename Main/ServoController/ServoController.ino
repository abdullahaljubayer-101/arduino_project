#include <ESP32Servo.h>

#define servoPin 15

Servo servo;

void setup() {
  Serial.begin(115200);

  servo.attach(servoPin);
}

void loop() {  
  if (Serial.available()) {
    int angle = Serial.parseInt();
    Serial.println(angle);
    servo.write(angle);
  }

  delay(20);
}
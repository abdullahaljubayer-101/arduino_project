#include <Adafruit_Fingerprint.h>

#define mySerial Serial2

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  delay(100);

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
      ;
  }

  Serial.println("\n\nDeleting all fingerprint templates!");
  Serial.println("Press 'Y' key to continue");
}

void loop() {
  if (Serial.available() && (Serial.read() == 'Y')) {
    finger.emptyDatabase();

    Serial.println("Now database is empty :)");
  }
}

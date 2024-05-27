#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>
#include "dht.h"

#define SS_PIN 10
#define RST_PIN 9
#define relay 6
#define buzzer 7
#define officePin_1 4
#define officePin_2 5
#define dht_apin A0
#define tempPin 8
#define CheckMode 1
#define RegistrationMode 2

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int mode;

uint8_t id;
byte readCard[4];
String office_01 = "62C8E551";
String office_02 = "43955013";
String tagID = "";

dht DHT;

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  pinMode(officePin_1, OUTPUT);
  digitalWrite(buzzer, LOW);

  pinMode(officePin_2, OUTPUT);
  digitalWrite(buzzer, LOW);

  pinMode(tempPin, OUTPUT);
  digitalWrite(tempPin, LOW);

  Serial.begin(9600);
  while (!Serial)
    ;
  delay(100);
  SPI.begin();
  finger.begin(57600);
  mfrc522.PCD_Init();
  delay(4);

  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("------------------------------");
  Serial.println("RFID: OK");
  Serial.println("------------------------------");

  if (finger.verifyPassword()) {
    Serial.println("------------------------------");
    Serial.println("Fingerprint: OK");
    Serial.println("------------------------------");
  } else {
    Serial.println("------------------------------");
    Serial.println("Fingerprint: Not Found");
    Serial.println("------------------------------");
  }

  Serial.println("------------------------------");
  Serial.println("Mode: 1: Check | 2: Registration");
  Serial.println("------------------------------");
  mode = readnumber();
  if (mode == CheckMode) {
    Serial.println("------------------------------");
    Serial.println("Mode: Check");
    Serial.println("------------------------------");
  }
  if (mode == RegistrationMode) {
    Serial.println("------------------------------");
    Serial.println("Mode: Registration");
    Serial.println("------------------------------");
  }
}

void loop() {
  // Check
  if (mode == CheckMode) {
    // RFID Check
    if (RFID()) {
      if (tagID == office_01) {
        Serial.println("\nOffice - 01");
        digitalWrite(buzzer, HIGH);
        delay(200);
        digitalWrite(buzzer, LOW);
        digitalWrite(officePin_1, HIGH);
        delay(3000);
        digitalWrite(officePin_1, LOW);
      } else if (tagID == office_02) {
        Serial.println("\nOffice - 02");
        digitalWrite(buzzer, HIGH);
        delay(200);
        digitalWrite(buzzer, LOW);
        digitalWrite(officePin_2, HIGH);
        delay(3000);
        digitalWrite(officePin_2, LOW);
      } else {
        Serial.println("\nAccess Denied!");
        for (int i = 0; i < 3; i++) {
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          delay(200);
        }
      }
    }

    delay(500);

    // Fingerprint Check
    getFingerprintID();
    delay(500);


    // temp
    DHT.read11(dht_apin);
    if (DHT.temperature >= 26) {
      digitalWrite(tempPin, HIGH);
      delay(5000);
      digitalWrite(tempPin, LOW);
    }
  }

  // Registration
  if (mode == RegistrationMode) {
    FingerprintEnroll();
  }

  // temp();
}

void temp() {
  // DHT.read11(dht_apin);

  // Serial.print("Current humidity = ");
  // Serial.print(DHT.humidity);
  // Serial.print("%  ");
  // Serial.print("temperature = ");
  // Serial.print(DHT.temperature);
  // Serial.println("C  ");

  // delay(5000);
}

boolean RFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
    }
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("Found ID #");
  Serial.print(finger.fingerID);

  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);

  digitalWrite(relay, HIGH);
  delay(2000);
  digitalWrite(relay, LOW);

  return finger.fingerID;
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (!Serial.available())
      ;
    num = Serial.parseInt();
  }
  return num;
}

boolean FingerprintEnroll() {
  Serial.println("------------------------------");
  Serial.println("Fingerprint: Enroll | ID # (1 to 127)");
  Serial.println("------------------------------");
  id = readnumber();
  if (id == 0) {
    return;
  }
  Serial.println("------------------------------");
  Serial.print("Fingerprint: Enrolling | ID # ");
  Serial.println(id);
  Serial.println("------------------------------");
  Serial.println("\nPlace a finger");

  while (!getFingerprintEnroll())
    ;
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  Serial.println("\nPlace same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  Serial.println("Remove finger");

  Serial.print("\nCreating model for ID: ");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID: ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!\n");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
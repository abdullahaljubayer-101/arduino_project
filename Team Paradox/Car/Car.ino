char input;

int motorLeftF = 8;
int motorLeftB = 9;
int motorRightF = 10;
int motorRightB = 11;

void setup() {       
  Serial.begin(9600);

  pinMode(motorLeftF, OUTPUT);
  pinMode(motorLeftB, OUTPUT);
  pinMode(motorRightF, OUTPUT);
  pinMode(motorRightB, OUTPUT);
}

void loop() {
  if(Serial.available() > 0) {
    input = Serial.read();
    Serial.println(input);
    
    stop();

    if (input == 'F')
      forward();
    else if (input == 'B')
      back();
    else if (input == 'L')
      left();
    else if (input == 'R')
      right();
    else if (input == 'T')
      stop();
  }
}

void forward() {
  digitalWrite(motorLeftF, HIGH);
  digitalWrite(motorLeftB, LOW);
  digitalWrite(motorRightF, HIGH);
  digitalWrite(motorRightB, LOW);
}

void back() {
  digitalWrite(motorLeftF, LOW);
  digitalWrite(motorLeftB, HIGH);
  digitalWrite(motorRightF, LOW);
  digitalWrite(motorRightB, HIGH);
}

void left() {
  digitalWrite(motorLeftF, LOW);
  digitalWrite(motorLeftB, LOW);
  digitalWrite(motorRightF, HIGH);
  digitalWrite(motorRightB, LOW);
}

void right() {
  digitalWrite(motorLeftF, HIGH);
  digitalWrite(motorLeftB, LOW);
  digitalWrite(motorRightF, LOW);
  digitalWrite(motorRightB, LOW);
}

void stop() {
  digitalWrite(motorLeftF, LOW);
  digitalWrite(motorLeftB, LOW);
  digitalWrite(motorRightF, LOW);
  digitalWrite(motorRightB, LOW);
}

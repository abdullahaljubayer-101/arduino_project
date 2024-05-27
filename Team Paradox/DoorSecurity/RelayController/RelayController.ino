char input;
int relay = 15;

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    input = Serial.read();
    Serial.println(input);
    
    if (input == 'O') {
      digitalWrite(relay, HIGH);
    } else if (input == 'C') {
      digitalWrite(relay, LOW);
    }
  }
}

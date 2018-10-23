int led1 = D0;
int infraredDetector = A1;

void setup() {
  pinMode(led1, OUTPUT);
}

void loop() {
  digitalWrite(led1, HIGH);
  delay(3000);
  Particle.publish("detecting", String(analogRead(infraredDetector)));
}

int count;

void setup() {
  Serial.begin(9600);
  pinMode(3,INPUT);
}

void loop() {
  Serial.print("Sensor: ");
  Serial.println(digitalRead(3));
  delay(500);
}

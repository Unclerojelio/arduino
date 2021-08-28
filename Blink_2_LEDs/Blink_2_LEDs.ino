/*  Blink 2 LEDs
    Blink LEDs PB0 and PB1 for a defined delay.
    This simple sketch was made to showcase and test
    the testing socket on an ATTiny shield.
*/

#define del 50
  
void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(0, HIGH);
  delay(del);
  digitalWrite(1, HIGH);
  delay(del);
  digitalWrite(0, LOW);
  delay(del);
  digitalWrite(1, LOW);
  delay(del);
}

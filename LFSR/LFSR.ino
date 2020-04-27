// This sketch outputs the sequence of numbers generated by a
// Linear Feedback Shift Register
//
// Roger Banks
// 2020 04 21
//

int sr[16];
int temp;

void shift_right() {
  for(int i = 3; i > 0; i--) {
    sr[i] = sr[i-1];
  }
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for(int i = 2; i <=5; i++) pinMode(i, OUTPUT);
  for(int i = 3; i >= 0; i--) {
    sr[i] = 1<<i;
    //Serial.println(sr[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  temp = sr[2] ^ sr[3];
  shift_right();
  sr[0] = temp;
  Serial.println(temp);
  digitalWrite(2, temp & B0001);
  digitalWrite(3, temp & B0010);
  digitalWrite(4, temp & B0100);
  digitalWrite(5, temp & B1000);
  delay(500);
}

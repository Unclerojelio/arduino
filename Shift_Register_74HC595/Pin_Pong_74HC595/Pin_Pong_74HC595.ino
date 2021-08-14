/*
This code lights up each LED connected to a 74HC595 as determined by the binary value of a counter. */
//Pin connected to SRCLK of 74HC595 
int CLOCK = 12; // Pin connected to RCLK of 74HC595
int LATCH = 11; // Pin connected to SER of 74HC595 
int DATA = 10;

byte counter = 0;

int pattern[14] = {1,2,4,8,16,32,64,128,64,32,16,8,4,2};

void setup() { 
//set pins to output so you can control the shift register 
pinMode(LATCH, OUTPUT); 
pinMode(CLOCK, OUTPUT); 
pinMode(DATA, OUTPUT); 
}

void loop() {
  if(counter > 13) counter = 0;
  // take the latchPin low 
  digitalWrite(LATCH, LOW); 
  // shift out the bits: 
  shiftOut(DATA, CLOCK, MSBFIRST, pattern[counter]);
  //take the latch pin high so the LEDs update: 
  digitalWrite(LATCH, HIGH); 
  // pause before next value: 
  delay(100);
  
  counter = counter + 1;

}

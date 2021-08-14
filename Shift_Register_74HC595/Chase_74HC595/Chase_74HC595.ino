/*
This code lights up each LED connected to a 74HC595 as determined by the binary value of a counter. */
//Pin connected to SRCLK of 74HC595 
int CLOCK = 12; // Pin connected to RCLK of 74HC595
int LATCH = 11; // Pin connected to SER of 74HC595 
int DATA = 10;

byte counter = 0;

byte data_out = B00000001;

void setup() { 
//set pins to output so you can control the shift register 
pinMode(LATCH, OUTPUT); 
pinMode(CLOCK, OUTPUT); 
pinMode(DATA, OUTPUT); 
}

void loop() {
  if(counter > 7) counter = 0;
    
  // take the latchPin low 
  digitalWrite(LATCH, LOW); 
  // shift out the bits: 
  shiftOut(DATA, CLOCK, MSBFIRST, data_out << counter);
  //take the latch pin high so the LEDs update: 
  digitalWrite(LATCH, HIGH); 
  // pause before next value: 
  delay(100);
  
  counter++;

}

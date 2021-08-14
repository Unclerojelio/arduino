/*
This code lights up each LED connected to a 74HC595 as determined by the binary value of a counter. */
//Pin connected to SRCLK of 74HC595 
int CLOCK = 12; // Pin connected to RCLK of 74HC595
int LATCH = 11; // Pin connected to SER of 74HC595 
int DATA = 10;

byte counter = 0;

const int ARRAY_SIZE = 6;

//byte test = 0b00000001;
//byte test2 = B00000001;

byte pattern[ARRAY_SIZE] = { B10000001,
                             B01000010,
                             B00100100,
                             B00011000,
                             B00100100,
                             B01000010 };

void setup() { 
//set pins to output so you can control the shift register 
pinMode(LATCH, OUTPUT); 
pinMode(CLOCK, OUTPUT); 
pinMode(DATA, OUTPUT); 
}

void loop() {
  if(counter > ARRAY_SIZE - 1) counter = 0;
  // take the latchPin low 
  digitalWrite(LATCH, LOW); 
  // shift out the bits: 
  shiftOut(DATA, CLOCK, MSBFIRST, pattern[counter]);
  //take the latch pin high so the LEDs update: 
  digitalWrite(LATCH, HIGH); 
  // pause before next value: 
  delay(100);
  
  counter++;

}

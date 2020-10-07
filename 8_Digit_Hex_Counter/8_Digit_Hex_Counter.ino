// This sketch outputs the numbers of a 32-bit LFSR to an 8 digit 7 segment display.
// 
//
// Roger Banks
// 2020 10 01
//
// Need to add actual actual feedback code. The sketch is currently just shifting in random numbers.
// Algorithm:
//  1) Shift the required bits( 32, 22, 2, 1) out to four variables (bit_32, bit_22, bit_2, bit_1).
//  2) theFeedback = bit_32 ^ bit_22 ^ bit_2 ^ bit_1
//  3) Shift theNumber one bit to the left.
//  4) theNumber = theNumber + theFeedback


//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(5,7,6,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}


unsigned long theNumber = 0; // The actual shift register. This may need to change to bit array later.

void countDigits() {
  lc.setDigit(0, 7, theNumber       >> 28, false);
  lc.setDigit(0, 6, theNumber << 4  >> 28, false);
  lc.setDigit(0, 5, theNumber << 8  >> 28, false);
  lc.setDigit(0, 4, theNumber << 12 >> 28, false);
  lc.setDigit(0, 3, theNumber << 16 >> 28, false);
  lc.setDigit(0, 2, theNumber << 20 >> 28, false);
  lc.setDigit(0, 1, theNumber << 24 >> 28, false);
  lc.setDigit(0, 0, theNumber << 28 >> 28, false);
  theNumber++;
  delay(delaytime);
}

void loop() { 

  countDigits();
  
}

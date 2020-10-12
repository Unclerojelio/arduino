#include <FastLED.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 

// How many leds are in the strip?
#define NUM_LEDS 60
#define DATA_PIN 5

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

unsigned long delaytime=250;
unsigned long theNumber = 1; // The actual shift register. This may need to change to bit array later.

unsigned int bit_24, bit_23, bit_22, bit_17, bit_0;
unsigned int redValue, greenValue, blueValue;

// This function sets up the ledsand tells the controller about them
void setup() {
  //Serial.begin(9600);
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
 	delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  leds[0].setRGB(255, 0, 0);
  FastLED.show();
}

void loop() {

  bit_24  = theNumber       >> 23;
  bit_23  = theNumber << 1  >> 23;
  bit_22 =  theNumber << 2  >> 23;
  bit_17  = theNumber << 7  >> 23;
  
  bit_0 = bit_24 ^ bit_23 ^ bit_22 ^ bit_17;
  theNumber += bit_0;
  
  redValue   = theNumber       >> 16;
  greenValue = theNumber << 8  >> 16;
  blueValue  = theNumber << 16 >> 16;
  
  
  for(int whiteLed = NUM_LEDS-2; whiteLed >= 0; whiteLed--) {
    leds[whiteLed+1] = leds[whiteLed];
    }
  
  leds[0].setRGB(redValue, greenValue, blueValue);
  
  FastLED.show(); 
  delay(delaytime);
  theNumber = theNumber << 1;
  //Serial.println(theNumber);
}

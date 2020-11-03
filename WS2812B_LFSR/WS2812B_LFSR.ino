// This sketch generates a 24 bit LFSR and then sends the data out as 3 8 bit values to a WS2812 LED strip.
//
// Roger Banks
// 2020 10 11
//

// Use the FastLED library
#include <FastLED.h>

#define NUM_LEDS 60 // number of LEDs in the WS2812 LED strip
#define DATA_PIN 5  // Arduino pin used to send data to the WS2812 LED strip

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

unsigned long delaytime=250; // delay between updates
unsigned long theNumber = 1; // The actual shift register. Initialize with a 1 otherwise the LFSR will only contain zeros.

unsigned int bit_24, bit_23, bit_22, bit_17, bit_0;
unsigned int redValue, greenValue, blueValue;

void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
 	delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  //leds[0].setRGB(255, 0, 0);
  //FastLED.show();
}

void loop() {

  // Extract the bits for the feedback function
  bit_24  = theNumber       >> 23;
  bit_23  = theNumber << 1  >> 23;
  bit_22 =  theNumber << 2  >> 23;
  bit_17  = theNumber << 7  >> 23;
  
  bit_0 = bit_24 ^ bit_23 ^ bit_22 ^ bit_17; // the feedback function
  theNumber += bit_0; // insert the feeedback back into the shift regsiter

  // extract the RGB values from the shift register
  redValue   = theNumber       >> 16;
  greenValue = theNumber << 8  >> 16;
  blueValue  = theNumber << 16 >> 16;
  
  // move the LED values down the strip one place
  for(int whiteLed = NUM_LEDS-2; whiteLed >= 0; whiteLed--) {
    leds[whiteLed+1] = leds[whiteLed];
    }

  // set the first LED with the new color values
  leds[0].setRGB(redValue, greenValue, blueValue);
  
  FastLED.show(); 
  delay(delaytime);
  theNumber = theNumber << 1; // shift the register by 1
}

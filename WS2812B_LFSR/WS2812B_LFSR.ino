// This sketch generates a 24 bit LFSR and then sends the data out as 3 8 bit values to a WS2812 LED strip. The result
// is a pseudo-random sequence of colors that shift down the LED strip.
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

unsigned int bit_24, bit_23, bit_22, bit_17, bit_1;        // Historically, shift registers are numbered 1-N
unsigned int redValue, greenValue, blueValue;

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  set_max_power_in_volts_and_milliamps(5, 3000);            // This particular setup can deliver 3A at 5V. Do not run this
                                                            // directly from a USB port without lowering the brightness or
                                                            // number of LEDs.
  set_max_power_indicator_LED(LED_BUILTIN);                 // Flash the builtin LED if the brightness is being lowered.
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
}

void loop() {

  // Extract the bits for the feedback function
  bit_24  = theNumber       >> 23;
  bit_23  = theNumber << 1  >> 23;
  bit_22 =  theNumber << 2  >> 23;
  bit_17  = theNumber << 7  >> 23;
  
  bit_1 = bit_24 ^ bit_23 ^ bit_22 ^ bit_17;   // the feedback function
  bit_1 = bit_1 & 1;
  
  theNumber = theNumber << 1;                  // shift the register by 1
  theNumber += bit_1;                          // insert the feeedback back into the shift register

  // this is incorrect. the values need to be bytes in order to be the correct 8 bit values.
  // extract the RGB values from the shift register
  redValue   = theNumber       >> 16;
  greenValue = theNumber << 8  >> 16;
  blueValue  = theNumber << 16 >> 16;
  
  // shift the LED colors down the strip by one pixel
  for(int currentLed = NUM_LEDS-2; currentLed >= 0; currentLed--) {
    leds[currentLed+1] = leds[currentLed];
  }

  // set the first LED with the new color values
  //leds[0].setRGB(redValue, greenValue, blueValue);

  if(bit_1) leds[0] = CRGB::White;
  else leds[0] = CRGB::Black;
  
  FastLED.show(); 
  delay(delaytime);
}

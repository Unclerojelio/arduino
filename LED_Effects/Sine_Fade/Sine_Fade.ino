/**************************************************************************
*
* SINE_Fade
* PWM an led up and down with a sine waveform.
* Roger Banks roger_banks@mac.com
* May 25, 2014
***************************************************************************/

int outputLED = 11;
int onboardLED = 13;
const int DELAY = 5;
int lookup[255]; // sine value lookup table

void setup() {
  pinMode(outputLED, OUTPUT);
  pinMode(onboardLED, OUTPUT);
  digitalWrite(onboardLED, LOW); //Turn the onboard LED off
  float dx = 1.411; // scaling factor to map 360 degrees to the 255 values of the PWM range (360 / 255)
  for(int i = 0; i < 255; i++) {
    lookup[i] = int((sin(i * dx * PI/180) + 1) * 128); // Convert radians to degrees (dx * PI/180)
                                                       // Map index to 360 degrees(i * dx)
                                                       // calculate the sine value
                                                       // Shift waveform up to positive values ( +1)
                                                       // Map the value to 255 (full range)
  }
}

void loop() {
  for(int i = 0; i < 255; i++) {
    analogWrite(outputLED, lookup[i]);
    delay(DELAY);
  }
}

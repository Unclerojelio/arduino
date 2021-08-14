/**************************************************************************
*
* PWM_test
* PWM an led up and down with a sawtooth waveform.
* Roger Banks roger_banks@mac.com
* May 25, 2014
***************************************************************************/

int outputLED = 11;
int onboardLED = 13;
const int DELAY = 5;

void setup() {
  pinMode(outputLED, OUTPUT);
  pinMode(onboardLED, OUTPUT);
  digitalWrite(onboardLED, LOW); //Turn the onboard LED off
}

void loop() {
  for(int i = 0; i < 255; i++) {
    analogWrite(outputLED, i);
    delay(DELAY);
  }
  analogWrite(outputLED, 255);
  for(int i = 255; i > 0; i--) {
    analogWrite(outputLED, i);
    delay(DELAY);
  }
}

#define LED_COUNT 3

#define LED1_PIN  5
#define LED2_PIN  7
#define LED3_PIN  9

// Each element in the array follows the format { PIN, BRIGHTNESS }
byte _ledArray[][2] = {
  { LED1_PIN, 0},
  { LED2_PIN, 0},
  { LED3_PIN, 0}
};

volatile byte _bamCount;

void setup() {

  // Set all the led pins to be outputs.
  for (byte i = 0; i < LED_COUNT; i++) {
    pinMode(_ledArray[i][0], OUTPUT);
  }
  
  // Setup timer2 to cause an interrupt every 640 microseconds.  1024 prescaler / 16000000 clock frequency Hz * 10 counts
  TCCR2A = _BV(WGM21);  // CTC
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20); // 1024 prescaler
  OCR2A = 10;           // Output Compare Register A
  TIMSK2 = _BV(OCIE2A); // Output Compare Interrupt Enable Match A 
}

void loop() {
  
  analogWriteBam(LED1_PIN, 1);
  delay(1000);
  analogWriteBam(LED2_PIN, 4);
  delay(1000);
  analogWriteBam(LED3_PIN, 15);
  delay(1000);
  
  analogWriteBam(LED1_PIN, 0);
  analogWriteBam(LED2_PIN, 0);
  analogWriteBam(LED3_PIN, 0);
  delay(1000);
}

void analogWriteBam(byte pin, byte brightness) {

  // Look for the pin in the array.
  for (byte i = 0; i < LED_COUNT; i++) {
    if (_ledArray[i][0] == pin) {
      _ledArray[i][1] = brightness; // Save the brightness.
      break;                        // Stop looking.
    }
  }
}

ISR(TIMER2_COMPA_vect) {
  
  _bamCount++;
  if (_bamCount > 15) _bamCount = 1; // Ensure counter goes from 1-15.
  
  if      (_bamCount == B0001) { updateLeds(0); }
  else if (_bamCount == B0010) { updateLeds(1); }
  else if (_bamCount == B0100) { updateLeds(2); }
  else if (_bamCount == B1000) { updateLeds(3); }
}

void updateLeds(byte bitPosition) {
  for (byte i = 0; i < LED_COUNT; i++) {
    byte pin = _ledArray[i][0];
    byte brightness = _ledArray[i][1];
    digitalWrite(pin, bitRead(brightness, bitPosition));
  }
}

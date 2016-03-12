#define PIN_LED 13

volatile byte _bamCount;
volatile byte _ledBrightness;
volatile byte _increaseBrightness;
unsigned long _currentMillis, _lastUpdateMillis;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  
  // Setup timer2 to cause an interrupt every 640 microseconds.  1024 prescaler / 16000000 clock frequency Hz * 10 counts
  TCCR2A = _BV(WGM21);  // CTC
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20); // 1024 prescaler
  OCR2A = 10;           // Output Compare Register A
  TIMSK2 = _BV(OCIE2A); // Output Compare Interrupt Enable Match A 
}

void loop() {
  _currentMillis = millis();
  
  // Once every 50 milliseconds, change the led brightness.
  // It starts at 0 and goes up to 15, then back down to 0, and repeats.
  if (_currentMillis - _lastUpdateMillis > 49) {
    _lastUpdateMillis = _currentMillis;
        
    if      (_ledBrightness == 0)  _increaseBrightness = true;
    else if (_ledBrightness == 15) _increaseBrightness = false;
    
    if (_increaseBrightness) 
      _ledBrightness++;
    else
      _ledBrightness--;
  }
}

ISR(TIMER2_COMPA_vect) { // timer interrupt service routine

  _bamCount++;
  if (_bamCount > 15) _bamCount = 1; // Ensure counter goes from 1-15.
  
  if      (_bamCount == B0001) { updateLeds(0); }
  else if (_bamCount == B0010) { updateLeds(1); }
  else if (_bamCount == B0100) { updateLeds(2); }
  else if (_bamCount == B1000) { updateLeds(3); }
}

void updateLeds(byte bitPosition) {
  digitalWrite(PIN_LED, bitRead(_ledBrightness, bitPosition));
}

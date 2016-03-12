#define PIN_LED 9

byte _bamCount;
byte _ledBrightness;
unsigned long _currentMillis, _lastUpdateMillis;

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  
  _currentMillis = millis();
  
  // Once every 50 milliseconds, change the led brightness.
  // It cannot go over 15 so when it gets that bright, change it back to 0.
  if (_currentMillis - _lastUpdateMillis > 49) {
    _lastUpdateMillis = _currentMillis;
    _ledBrightness++;
    if (_ledBrightness > 15) _ledBrightness = 0;
  }
  
  bitAngleModulationHandler();
}

void bitAngleModulationHandler() {

  _bamCount++;
  
  if (_bamCount > B1111) _bamCount = B0001; // Ensure counter goes from 1 to 15
  
                                                  // if led brightness is 10 (binary B1010)
  if      (_bamCount == B0001) { updateLeds(0); } // cycle  1 led off
  else if (_bamCount == B0010) { updateLeds(1); } // cycle  2 led on
                     // B0011                        cycle  3 led still on
  else if (_bamCount == B0100) { updateLeds(2); } // cycle  4 led off
                     // B0101                        cycle  5 led still off
                     // B0110                        cycle  6 led still off
                     // B0111                        cycle  7 led still off
  else if (_bamCount == B1000) { updateLeds(3); } // cycle  8 led on
                     // B1001                        cycle  9 led still on
                     // B1010                        cycle 10 led still on
                     // B1011                        cycle 11 led still on
                     // B1100                        cycle 12 led still on
                     // B1101                        cycle 13 led still on
                     // B1110                        cycle 14 led still on
                     // B1111                        cycle 15 led still on
                     
                     // So out of the available 15 cycles, a brightness of 10 sets the
                     // led to be on for 10 of them.
}

void updateLeds(byte bitPosition) {
  
  // if brightness is 10 (binary B1010) and bit position is 0
  // bitRead(B1010, 0) = 0 = false
  
  bool isEnabled = bitRead(_ledBrightness, bitPosition);
  digitalWrite(PIN_LED, isEnabled);
}

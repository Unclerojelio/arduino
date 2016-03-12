// Arduino digital pins.
byte _ledArray[] = { 5, 6, 7, 8, 9, 13 };

byte _ledCount;
byte _bamCount = 0;
byte _ledBrightness = 0;
unsigned long _currentMillis, _lastUpdateMillis;
unsigned long _currentMicros, _lastUpdateMicros;

void setup() {

  // Determine number of led's in the array.
  _ledCount = sizeof(_ledArray);
  
  // Set all the led pins to be outputs.
  for (byte i = 0; i < _ledCount; i++) {
    pinMode(_ledArray[i], OUTPUT);
  }  
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

  // To make the led brightness increase correctly, we ensure each bam count
  // lasts the same amount of time.  While testing it was discovered that a
  // brightness of 3 would be brighter than a brightness of 4 because the
  // microcontroller needed slightly more time to perform the on/off cycles
  // with the brightness of 3 vs the brightness of 4.  A brightness of 3
  // takes 2 calls to updateLeds whereas the brightness of 4 only required
  // 1 call, and in the end the brighness of 4 did not last long enough to
  // make the led appear brighter.
  _currentMicros = micros();

  // Also during testing I found the duration of each bam count could be increased
  // to 700 microseconds without any noticeable flicker.  The longer the duration,
  // the more time the microcontroller can do other work, but if we make it too long
  // the refresh rate will be too slow and we see flicker.
  if (_currentMicros - _lastUpdateMicros > 699) {
  
    _lastUpdateMicros = _currentMicros;
    _bamCount++;
    if (_bamCount > 15) _bamCount = 1; // Ensure counter goes from 1-15.
    
    if      (_bamCount == B0001) { updateLeds(0); }
    else if (_bamCount == B0010) { updateLeds(1); }
    else if (_bamCount == B0100) { updateLeds(2); }
    else if (_bamCount == B1000) { updateLeds(3); }
  }
}

void updateLeds(byte bitPosition) {
  for (byte i = 0; i < _ledCount; i++) {  
    byte pin = _ledArray[i];
    bool isEnabled = bitRead(_ledBrightness, bitPosition);
    digitalWrite(pin, isEnabled);
  }
}

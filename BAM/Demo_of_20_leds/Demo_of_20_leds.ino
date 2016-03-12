// If doing this on Arduino, be sure to measure the current draw of 1 led + resistor
// to ensure you do not overload the microcontroller since you will be using all its pins.
// Read the limits here:  http://playground.arduino.cc/Main/ArduinoPinCurrentLimitations

// Each element in the array follows the format { PIN, BRIGHTNESS, MISC }
// PIN is the Arduino digital pin number 
// BRIGHTNESS is 0-15
// MISC can be used for anything
byte _ledArray[][3] = {
  { A5, 0, 1},
  { A4, 0, 1},
  { A3, 0, 1},
  { A2, 0, 1},
  { A1, 0, 1},
  { A0, 0, 1},
  {  0, 0, 1},
  {  1, 0, 1},
  {  2, 0, 1},
  {  3, 0, 1},
  {  4, 0, 1},
  {  5, 0, 1},
  {  6, 0, 1},
  {  7, 0, 1},
  {  8, 0, 1},
  {  9, 0, 1},
  { 10, 0, 1},
  { 11, 0, 1},
  { 12, 0, 1},
  { 13, 0, 1}
};

byte _ledCount, _bamCounter, _current, _direction;
unsigned long _currentMillis, _lastMillis, _bamMicros, _lastBrightChangeMillis;

void setup() {
  _ledCount = sizeof(_ledArray) / 3; // Each array element is 3 bytes.
  randomSeed(analogRead(A0));        // Randomize the random number generator.
  
  // Set all the led pins to be outputs.
  for (byte led = 0; led < _ledCount; led++) {
    pinMode(_ledArray[led][0], OUTPUT); // Array index 0 = the Arduino pin number.
  }
}

void loop() {
  doRunner(); 
  doRandomSet(); 
  doFadeAll();
}

void bamHandler() {
  unsigned long currentMicros = micros();
  if (currentMicros - _bamMicros > 699) {
    _bamMicros = currentMicros;
    _bamCounter++;
    if (_bamCounter > B1111) _bamCounter = 1; // Ensure count goes from 1 to 15
    
    if      (_bamCounter == B0001) { updateLeds(0); }
    else if (_bamCounter == B0010) { updateLeds(1); }
    else if (_bamCounter == B0100) { updateLeds(2); }
    else if (_bamCounter == B1000) { updateLeds(3); }
  }
}

void updateLeds(byte bitPosition) {
  for (byte led = 0; led < _ledCount; led++) {
    digitalWrite(_ledArray[led][0], bitRead(_ledArray[led][1], bitPosition));
  }
}

void doRunner() {

  // Turns on 1 led at a time, fading it up and down, then moves on to the next.
  // There's a timeout so the demonstration only lasts a certain amount of time.

  unsigned long timeout = millis() + 7000;

  while (timeout > millis()) {
    _currentMillis = millis();
  
    if (_currentMillis - _lastMillis > 1000) {
      _lastMillis = _currentMillis;
      
      // Turn off every led.
      for (byte led = 0; led < _ledCount; led++) {
        _ledArray[led][1] = 0;
      }
  
      // Increment the led index that we'll be lighting.
      _current++;
      if (_current > _ledCount) {
        _current = 0;
        break;
      }
    }
    
    // Fade the current led up and down.
    if (_currentMillis - _lastBrightChangeMillis > 29) {
      _lastBrightChangeMillis = _currentMillis;
      
      if (_ledArray[_current][1] == 0) _direction = 1;
      if (_ledArray[_current][1] == 15) _direction = 0;
  
      if (_direction)
        _ledArray[_current][1]++;
      else
        _ledArray[_current][1]--;
    }

    bamHandler();
  }
}

void doRandomSet() {

  // Picks 5 leds at random and fades them up and down, then repeats.
  // There's a timeout so the demonstration only lasts a certain amount of time.
  
  // Each element in the array follows the format { PIN, BRIGHTNESS, BRIGHTNESS FLAG } where
  // BRIGHTNESS FLAG = 0 do not adjust the led's brightness
  // BRIGHTNESS FLAG = 1 increase the led's brightness
  // BRIGHTNESS FLAG = 2 decrease the led's brightness
  
  unsigned long timeout = millis() + 5000;
  
  while (timeout > millis()) {   

    // Turn off every led.
    for (byte led = 0; led < _ledCount; led++) {
      _ledArray[led][1] = 0; // BRIGHTNESS = 0
      _ledArray[led][2] = 0; // BRIGHTNESS FLAG = 0 = do not adjust this led
    }

    // Randomly pick leds to fade up and down by setting BRIGHTNESS FLAG = 1
    for (byte i = 0; i < 6; i++) {
      _ledArray[random(_ledCount)][2] = 1;
    }

    bool exitFlag = false;
    
    while (!exitFlag) {

      _currentMillis = millis();
      
      if (_currentMillis - _lastBrightChangeMillis > 49) {
        _lastBrightChangeMillis = _currentMillis;

        for (byte led = 0; led < _ledCount; led++) {
          if (_ledArray[led][2] == 1) { // BRIGHTNESS FLAG = 1 = increase it
            _ledArray[led][1]++;
            if (_ledArray[led][1] == 15) _ledArray[led][2] = 2; // When we hit the max brightness, set BRIGHTNESS FLAG = 2 = decrease it
          }
          else if (_ledArray[led][2] == 2) { // BRIGHTNESS FLAG = 2 = decrease it
            _ledArray[led][1]--;
            if (_ledArray[led][1] == 0) exitFlag = true; // We have completed the full up and down cycle, we exit and select a new set of leds to fade.
          }
        }
      }
  
      bamHandler();
    }
  }
}

void doFadeAll() {

  // Fades every led up and down where every other led fades up, and the others fade down.
  // There's a timeout so the demonstration only lasts a certain amount of time.

  // Each element in the array follows the format { PIN, BRIGHTNESS, BRIGHTNESS FLAG } where
  // BRIGHTNESS FLAG = 1 increase the led's brightness
  // BRIGHTNESS FLAG = 0 decrease the led's brightness
  
  #define MAX_BRIGHTNESS 7

  unsigned long timeout = millis() + 5000;

  // Setup initial brightness of each led in an alternating fashion.  1st will be off, 2nd fully bright, 3rd off, 4th fully bright, etc.
  for (byte led = 0; led < _ledCount; led++) {
    if (led % 2 == 0) { // modulo division
      _ledArray[led][1] = 0; // BRIGHTNESS = 0
    }
    else {
      _ledArray[led][1] = MAX_BRIGHTNESS;
    }
  }

  while (timeout > millis()) {
    _currentMillis = millis();
  
    if (_currentMillis - _lastBrightChangeMillis > 49) {
      _lastBrightChangeMillis = _currentMillis;
     
      for (byte led = 0; led < _ledCount; led++) {
        
        if      (_ledArray[led][1] == 0)              _ledArray[led][2] = 1; // BRIGHTNESS FLAG = increase
        else if (_ledArray[led][1] == MAX_BRIGHTNESS) _ledArray[led][2] = 0; // BRIGHTNESS FLAG = decrease
        
        if (_ledArray[led][2]) // Increase or decreate the led's brightness
          _ledArray[led][1]++;
        else
          _ledArray[led][1]--;
      }   
    }

    bamHandler();
  }
}

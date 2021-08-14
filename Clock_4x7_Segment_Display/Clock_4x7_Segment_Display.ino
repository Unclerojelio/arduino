/*****************************************************************************
* This sketch demonstrates the LD-5461AS Common Anode 4x7 Segement LED Display
*
* Roger Banks
* roger_banks@mac.com
* 09 Jun 2014
*
*******************************************************************************/

// Bit patterns to produce desired representation of a particular digit on
// the display.
const byte DIGITS[10] = { B00000000,   // 0
                          B00000001,   // 1
                          B00000010,   // 2
                          B00000011,   // 3
                          B00000100,   // 4
                          B00000101,   // 5
                          B00000110,   // 6
                          B00000111,   // 7
                          B00001000,   // 8
                          B00001001 }; // 9
                          
// Arduino pin assignments                          
const int THOUSANDS = 0;
const int HUNDREDS = 1;
const int TENS = 2;
const int ONES = 3;
const int INPUT_A = 4;
const int INPUT_B = 5;
const int INPUT_C = 6;
const int INPUT_D = 7;

// THe four digit number to display.
int theNumber = 0;

// State variable
unsigned long previousMillis = 0;

// Time interval on which to increment display
const int TIME_INTERVAL = 100;

void setup() {
  
  // Setup arduino pins
  pinMode(ONES, OUTPUT);
  pinMode(TENS, OUTPUT);
  pinMode(HUNDREDS, OUTPUT);
  pinMode(THOUSANDS, OUTPUT);
  pinMode(INPUT_A, OUTPUT);
  pinMode(INPUT_B, OUTPUT);
  pinMode(INPUT_C, OUTPUT);
  pinMode(INPUT_D, OUTPUT);
  
  // Clear the display
  clearAll();
  
  //Turn off the pin 13 LED
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {

  // Increment the display if the time interval has expired
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > TIME_INTERVAL) {
    previousMillis = currentMillis;
    theNumber++;
  }
  
  //Make sure there are only 4 digits
  if(theNumber > 9999) theNumber = 0;
  // Write out the number to the display
  writeNumber(theNumber);
}

// Clear all segments of the display
void clearAll() {
  digitalWrite(ONES, LOW);
  digitalWrite(TENS, LOW);
  digitalWrite(HUNDREDS, LOW);
  digitalWrite(THOUSANDS, LOW);
  digitalWrite(INPUT_A, LOW);
  digitalWrite(INPUT_B, LOW);
  digitalWrite(INPUT_C, LOW);
  digitalWrite(INPUT_D, LOW);
}

// Write the number to the CD4511 input lines
void writeDigit(int place, int number) {
  if(DIGITS[number] & B00000001) digitalWrite(INPUT_A, HIGH);
  if(DIGITS[number] & B00000010) digitalWrite(INPUT_B, HIGH);
  if(DIGITS[number] & B00000100) digitalWrite(INPUT_C, HIGH);
  if(DIGITS[number] & B00001000) digitalWrite(INPUT_D, HIGH);
  digitalWrite(place, HIGH);
}

// Write the number to the display, one digit at a time
void writeNumber(int number) {
  number = abs(number);
  
  // Divide up the 4 digit number into its constituent digits
  int thousandsDigit = int(number / 1000);
  number = number % 1000;
  int hundredsDigit = int(number / 100);
  number = number % 100;
  int tensDigit = int(number / 10);
  number = number % 10;
  int onesDigit = number;
  
  // Write out the number
  clearAll();
  writeDigit(THOUSANDS, thousandsDigit);
  clearAll();
  writeDigit(HUNDREDS, hundredsDigit);
  clearAll();
  writeDigit(TENS, tensDigit);
  clearAll();
  writeDigit(ONES, onesDigit);
}

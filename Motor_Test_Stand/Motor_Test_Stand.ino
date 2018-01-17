/*
Motor_Test_Stand

by Roger Banks 15 Jan 2018

This sketch displays voltage, current, rpm and elapsed time of an electric motor under test. 

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int batMonPin = A4;         // input pin for the voltage divider
int batVal = 0;             // variable for the A/D value
float pinVoltage = 0;       // variable to hold the calculated voltage
float batteryVoltage = 0;   // final calculated battery voltage
int seconds = 0;            // will contain the number of seconds to display
int minutes = 0;            // will contain the number of minutes to display

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("RPM=");
  lcd.setCursor(9,0);
  lcd.print("A=");
  lcd.setCursor(9,1);
  lcd.print("V=");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the elapsed time since reset
  seconds = (millis() / 1000) % 60;
  minutes = (millis() / 1000) / 60;
  if (minutes < 10)
    lcd.print(0);
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10)
    lcd.print(0);
  lcd.print(seconds);
  //lcd.print(millis() / 1000);
  
  batVal = analogRead(batMonPin);    // read the voltage on the divider
  pinVoltage = batVal * .00488;
  batteryVoltage = pinVoltage * 6;
  lcd.setCursor(12,1);
  lcd.print(batteryVoltage);
}


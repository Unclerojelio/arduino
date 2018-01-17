/*
Motor_Test_Stand

by Roger Banks 15 Jan 2018

This sketch displays voltage, current, rpm and elapsed time of an electric motor under test.

Sensors used:
  RPM:     IR Obstacle Avoidance Sensor Module, ST-1081, from Iduino
  Voltage: Voltage Dividor using discrete 12K and 2K 1% resistors
  Current:
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Pin assignments
int batMonPin = A4;         // input pin for the voltage divider
//int irLED = 13;           // The infrared LED is connected to digital pin 13. Not used now but might use later
                            // if I switch from the IR module to separate IRLED and IR Phototransistor.

// Variable declarations
int batVal = 0;             // variable for the A/D value
float pinVoltage = 0;       // variable to hold the calculated voltage
float batteryVoltage = 0;   // final calculated battery voltage
int seconds = 0;            // will contain the number of seconds to display
int minutes = 0;            // will contain the number of minutes to display
volatile byte breakNum;        // "volatile" is used with interrupts
unsigned int rpm;

// Counts the number of interrupts
void break_count()
{
    breakNum++;
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("RPM=");
  lcd.setCursor(9,0);
  lcd.print("A=");
  lcd.setCursor(9,1);
  lcd.print("V=");

  //The Infrared phototransistor is connected to pin 2 which is interrupt 0.
  //Triggers on change from HIGH to LOW
  attachInterrupt(0, break_count, FALLING);
}

void loop() {

  // Update RPM every second
  delay(1000);

  // Don't process interrupts during calculations
  detachInterrupt(0);

  // Depending on what you are testing you might need to change the formula for the rpm
  // For instance testing a prop would give 2 breaks per rotation so (60 * rpmcount) / 2
  rpm = (60 * breakNum);

  breakNum = 0;

  // Print rpm in RPM field
  lcd.setCursor(4,0);
  lcd.print("0000");
  if(rpm < 100) lcd.setCursor(6,0);
  else if (rpm < 1000) lcd.setCursor(5,0);
  else lcd.setCursor(4,0);
  lcd.print(rpm);
   
  // set the cursor to time field
  lcd.setCursor(0, 1);
  // print, with leading zeros, the elapsed time since reset
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

  //Restart the interrupt processing
  attachInterrupt(0, break_count, FALLING);
}


/*
Motor_Test_Stand

by Roger Banks 15 Jan 2018

This sketch displays voltage, current, rpm and elapsed time of an electric motor under test.

Sensors used:
  RPM:     IR Obstacle Avoidance Sensor Module, ST-1081, from Iduino
  Voltage: Voltage Dividor using discrete 12K and 2K 1% resistors
  Current:

Todo:
  1) Add current sensor
  2) Add servo control to control ESC
  3) Add data logging
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Pin assignments
int batMonPin = A4;         // input pin for the voltage divider
int currentSensePin = A3;   // input pin for the ACS711EX Current Sensor
//int irLED = 13;           // The infrared LED is connected to digital pin 13. Not used now but might use later
                            // if I switch from the IR module to separate IRLED and IR Phototransistor.

// Variable declarations
int batVal = 0;             // variable for the A/D value
float pinVoltage = 0;       // variable to hold the calculated voltage of the voltage sensor pin
float currentPinVoltage = 0 // variable to hold the calculated voltage of the current sensor pin
float batteryVoltage = 0;   // final calculated battery voltage
float battertCurrent = 0;   // final calculated battery current
int Vcc = 5;
volatile byte breakNum;     // "volatile" is used with interrupts
unsigned int rpm;           // contains the calculated revolutions per minute adjusted for the number of propeller blades
String timeStr;             // contains the formatted time string

// Counts the number of interrupts
void break_count()
{
    breakNum++;
}

// Returns the current millis in "00:00" format
String millis2String(long int currentMillis)
{
  //Serial.println(currentMillis);
  String tempStr = "";
  int seconds = (currentMillis / 1000) % 60;
  int minutes = (currentMillis / 1000) / 60;
  if (minutes < 10)
    tempStr = tempStr + "0";
  tempStr = tempStr + String(minutes);
  tempStr = tempStr + ":";
  if (seconds < 10)
    tempStr = tempStr + "0";
  tempStr = tempStr + String(seconds);
  return tempStr;
}


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //Setup serial monitor and column headers 
  Serial.begin(9600);
  Serial.println("Time\tVolts\tRPM");

  //The Infrared phototransistor is connected to pin 2 which is interrupt 0.
  //Triggers on change from HIGH to LOW
  attachInterrupt(0, break_count, FALLING);
}

void loop() {

  // Update RPM every second
  delay(1000);

  // Don't process interrupts during calculations
  detachInterrupt(0);

  //Clear LCD and print labels
  lcd.clear();
  lcd.print("RPM=");
  lcd.setCursor(9,0);
  lcd.print("A=");
  lcd.setCursor(9,1);
  lcd.print("V=");

  // Depending on what you are testing you might need to change the formula for the rpm
  // For instance testing a prop would give 2 breaks per rotation so (60 * rpmcount) / 2
  rpm = (60 * breakNum);

  breakNum = 0;

  // Print rpm in RPM field
  lcd.setCursor(4,0);
  lcd.print(rpm);
   
  // set the cursor to time field
  lcd.setCursor(0, 1);
  // print, with leading zeros, the elapsed time since reset
  timeStr = millis2String(millis());
  lcd.print(timeStr);
  
  batVal = analogRead(batMonPin);       // read the output of the voltage sense A/D
  pinVoltage = batVal * .00488;         // map 0-1023 to 0-5V to calculate the voltage at the divider
  batteryVoltage = pinVoltage * 6;      // multiply by the voltage divider ratio
  batVal = analogRead(currentSensePin); // read the output of the current sense A/D
  currentPinVoltage = batVal * .00488;  // map 0-1023 to 0-5V to calculate the output voltage of the current sensor
  batteryCurrent = 73.3 * currentPinVoltage/Vcc - 36.7 // calculate final battery current
  
  lcd.setCursor(12,1);
  lcd.print(batteryVoltage);

  lcd.setCursor(12,0);
  lcd.print(batteryCurrent);

  Serial.print(timeStr);
  Serial.print("\t");
  Serial.print(batteryVoltage);
  Serial.print("\t");
  Serial.print(batteryCurrent);
  Serial.print("\t");
  Serial.println(rpm);

  //Restart the interrupt processing
  attachInterrupt(0, break_count, FALLING);
}

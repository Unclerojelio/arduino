/* Optical Tachometer
 *
 * The Infrared LED is connected to pin 13.
 * The Infrared phototransistor is connected to pin 2 which is interrupt 0.
 *
 */

int irLED = 13;                // The Infrared LED is connected to digital pin 13
volatile byte breakNum;        // "volatile" is used with interrupts
unsigned int rpm;

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Counts the number of interrupts
void break_count()
 {
      breakNum++;
 }

void setup()
 {
   // Was first using the serial port to print results.
   // Serial.begin(9600);
   lcd.begin(16,2);
   
   //The Infrared phototransistor is connected to pin 2 which is interrupt 0.
   //Triggers on change from HIGH to LOW
   attachInterrupt(0, break_count, FALLING);

   // Turn on IR LED
   pinMode(irLED, OUTPUT);
   digitalWrite(irLED, HIGH);

   breakNum = 0;
   rpm = 0;
 }

 void loop()
 {
   // Update RPM every second
   delay(1000);
   // Don't process interrupts during calculations
   detachInterrupt(0);
  // Depending on what you are testing you might need to change the formula for the rpm
  // For instance testing a prop would give 2 breaks per rotation so (60 * rpmcount) / 2
   rpm = (60 * breakNum);
   
  // Was using the serial port initially to print results
  // Serial.print(rpm);
  // Serial.println(" RPMs");
  // Serial.println();
  
   breakNum = 0;

   //Print out result to lcd
   lcd.clear();
   lcd.print("RPM=");
   lcd.print(rpm);
   
   //Restart the interrupt processing
   attachInterrupt(0, break_count, FALLING);
  }

#include "LedControl.h"

// LedControl lc = LedControl(Data,Clk,CS,numDevices)
LedControl lc = LedControl(5, 7, 6, 1);
 
int delaytime = 1;
unsigned long counter = 0;
unsigned long counter2 = 0;
 
void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);
}
 
void loop() {
  if (counter >= 4294967295) { // 2^32 - 1
    counter = 0;
    counter2++;
  }
  lc.setColumn(0, 0, counter);
  lc.setColumn(0, 1, counter >> 8);
  lc.setColumn(0, 2, counter >> 16);
  lc.setColumn(0, 3, counter >> 24);
  lc.setColumn(0, 4, counter2);
  lc.setColumn(0, 5, counter2 >> 8);
  lc.setColumn(0, 6, counter2 >> 16);
  lc.setColumn(0, 7, counter2 >> 24);
  delay(delaytime);
 
  counter++;
}

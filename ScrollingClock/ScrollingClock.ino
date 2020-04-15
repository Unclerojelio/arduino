// This project is to be a scrolling LED clock using the MAX7219 LED modules and the DS3231 RTC.
//
// Roger Banks
// 2020 04 13
//
// Current state: Arduino Pro mini plugged into breadboard with DS3231 module. The DS3231 module has been
// modified by removing the resistor providing charging current to the battery. The module has been tested
// to show that it retains the date after power removed.
//
// Todo:
//  Build up time string
//  Connect LED matrices to breadboard and include appropriate libraries and code
//  Display time, date and temp with various scrolling effects
//


#include <Wire.h>
#include <DS3231.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   7
#define DATA_PIN  5
#define CS_PIN    6

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

DS3231 Clock;
bool Century=false;
bool h12 = false;
bool PM;

int year, month, day, dow, hour, minute, second;
String timeString, dowString, monthString;
int counter = 0;

char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"};
char *months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jan","Jul","Aug","Sep","Oct","Nov", "Dec"};

String getDateString() {
  year = Clock.getYear();
  month = Clock.getMonth(Century);
  day = Clock.getDate();
  dow = Clock.getDoW();
  hour = Clock.getHour(h12, PM);
  minute = Clock.getMinute();
  second = Clock.getSecond();

  dowString = String(days[dow]);
  monthString = String(months[month-1]);

  return String("The current date is: " + dowString + ", " + monthString + " " + String(day) + " " + " " + String(year+2000));
}

String getTimeString() {
  String hourStr, minStr, secStr;
  
  hour = Clock.getHour(h12, PM);
  minute = Clock.getMinute();
  second = Clock.getSecond();

  hourStr = String(hour);
  minStr = String(minute);
  secStr = String(second);
  
  if (hour < 10) hourStr = "0" + hourStr;
  if (minute < 10) minStr = "0" + minStr;
  if (second < 10) secStr = "0" + secStr;

  return String("The current time is: " + hourStr + ":" + minStr + ":" + secStr);
}

String getNextString() {
  if (counter % 2 == 0) timeString = getDateString();
  else timeString = getTimeString();
  return timeString;
}

void setup() {
  // Start the I2C interface
  Wire.begin();
  // Start the serial interface
  //Serial.begin(9600);

  timeString = getDateString();
  P.begin();
  P.displayText(timeString.c_str(), PA_CENTER, 100, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

}

void loop() {
  
  if (P.displayAnimate())
  {
    timeString = getNextString();
    P.setTextBuffer(timeString.c_str());
    P.displayReset();
    counter += 1;
  }
  //P.displayAnimate();
}

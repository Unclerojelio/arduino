/*
 * Marquee using lcd.scrollDisplayLeft
 * this sketch scrolls up to 32 characters of text
 */

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int numRows = 2;
const int numCols = 16;


void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
}

void loop()
{
  marquee("A message too long to fit");
  delay(1000);
  lcd.clear();
}

// this function uses scrolling to display a message up to 32 bytes long
void marquee( char *text)
{  
  lcd.print(text);
  delay(1000);
  for (int position = 0; position < strlen(text)-numCols; position++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }    
}




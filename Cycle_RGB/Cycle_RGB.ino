/*
 File:        Cycle_RGB
 Description: Cycles RGB LED through the color wheel.
 By:          Roger Banks
 Date:        22 Nov 2015
 This example code is in the public domain.
*/

#define RED   3
#define GREEN 5
#define BLUE  6
#define SECS  1000  //delay

void setup() {                
 // initialize the digital pins as an outputs.
 pinMode(RED,   OUTPUT);  // Red
 pinMode(GREEN, OUTPUT);  // Green
 pinMode(BLUE,  OUTPUT);  // Blue
}

void loop() {
 digitalWrite(RED,   LOW);
 digitalWrite(GREEN, LOW);
 digitalWrite(BLUE,  LOW);
 digitalWrite(RED, HIGH);    // Red
 delay(SECS);            
 digitalWrite(GREEN, HIGH);  // Yellow
 delay(SECS); 
 digitalWrite(RED, LOW);     // Green
 delay(SECS);
 digitalWrite(BLUE, HIGH);   // Cyan
 delay(SECS);
 digitalWrite(GREEN, LOW);   // Blue  
 delay(SECS); 
 digitalWrite(RED, HIGH);    // Violet   
 delay(SECS);
}

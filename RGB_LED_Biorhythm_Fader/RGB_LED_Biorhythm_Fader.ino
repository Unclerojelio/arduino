// Taken from: http://www.instructables.com/id/Digispark-RGB-LED-Fader/step4/Arduino-Program/
// Filename: RGB_LED_Sine_Fader
// Modified by: Roger Banks roger_banks@mac.com
// 29 Feb 2016
// Changed output pins to work with and existing Arduino setup
// Changed rgb algorithm to the three biorythm formulas
// https://en.wikipedia.org/wiki/Biorhythm

// these variables will be used to hold the led PWM values
int RED_VALUE;
int GREEN_VALUE;
int BLUE_VALUE;

// Arduino pins
int RED_PIN   =3;
int GREEN_PIN =5;
int BLUE_PIN  =6;

// this a variable that will receive the angle value from variable i.
// This value is converted to radians in the sine function and will be used to generate the PWM values
float x;

void setup() {    

  // Initialize the digital pins as outputs.
  pinMode(RED_PIN,   OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);
  
  // Run a diagnostic test that will verify that each color of LED is working.
  digitalWrite(RED_PIN,   LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN,  LOW);
  
  digitalWrite(RED_PIN,   HIGH);  
  delay(1000);
  digitalWrite(RED_PIN,   LOW);
  digitalWrite(GREEN_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN,  HIGH);
  delay(1000);
}

void loop() {

  // 21252 is the LCD of {23,28,33}
  for (long i = 0; i < 21251; i++) {
  
    // Convert i into a floating point variable that can be used with PI
    x = float(i);
    
    // To calculate r,g,b the sine function is modified to increase amplitude (127*) to create a
    // phase shift (x+1/2*PI) and (x+3/2*PI) finally the sine wave is raised to eliminate negative values by adding 1
    RED_VALUE   = int(127*(sin(2*PI*x/23)+1)); // physical
    GREEN_VALUE = int(127*(sin(2*PI*x/28)+1)); // emotional
    BLUE_VALUE  = int(127*(sin(2*PI*x/33)+1)); // intellectual
    
    //write LED values to the output pins
    analogWrite (RED_PIN,   RED_VALUE);
    analogWrite (GREEN_PIN, GREEN_VALUE);
    analogWrite (BLUE_PIN,  BLUE_VALUE);
    
    delay(100);
  }
}

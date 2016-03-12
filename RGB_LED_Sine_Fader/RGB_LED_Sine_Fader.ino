// Taken from: http://www.instructables.com/id/Digispark-RGB-LED-Fader/step4/Arduino-Program/
// Filename: RGB_LED_Sine_Fader
// Modified by: Roger Banks roger_banks@mac.com
// 29 Feb 2016
// Changed output pins to work with and existing Arduino setup

//Declare all variables

int LED1; // these variables will be used to hold the led PWM values
int LED2;
int LED3;

int p0=3; /* these variables will assign a variable to receive PWM values and pass them to their respective pins*/
int p1=5;
int p4=6;

float x;
/*this a variable that will receive the angle value from variable i.
This value is converted to radians in the sine function and will be used to generate the PWM values */

float r; // these variables will receive the PWM values calculated by the three sine functions
float g;
float b;

// the setup routine runs once when you press reset;

void setup() {    

  // initialize the digitals pin as an output.
  pinMode(p0, OUTPUT); //sets up pin 0 for pwm
  pinMode(p1, OUTPUT); //sets up pin 1 for pwm
  pinMode(p4, OUTPUT); //sets up pin 4 for pwm
  
  /*Run a diagnostic test that will verify that each colour of LED is working.
  Turns on the LEDs consecutively with a delay of one second between each*/
  
  digitalWrite(p0, HIGH);  
  delay(1000);
  digitalWrite(p1, HIGH);
  delay(1000);
  digitalWrite(p4, HIGH);
  delay(1000);
  
  //Turn off LEDs one after the other with a one second deleay between each
  
  digitalWrite(p0, LOW);
  delay(1000);
  digitalWrite(p1, LOW);
  delay(1000);
  digitalWrite(p4, LOW);
  delay(1000);  
}

// the loop routine runs over and over again forever:
void loop() {

  /*The for loop generates a value for a variable i which corresponds to 0 to 360 degrees.
  I is increased by 1 with each iteration.  It is later converted to radians within the loop.
  Once i reaches 360 it resets back to 0. This establishes the periodic behaviour of the sine functions*/
  
  for (int i=0; i<360; i++)
  {
  
  //convert i into a floating point variable that can be used with PI
     x=float(i);
  
  /* to calculate r,g,b the sine function is modified to increase amplitute (127*) to create a
  phase shift (x+1/2*PI) and (x+3/2*PI) finally the sine wave is raised to illiminate negative values below zero by adding 1*/
  
     r=127*(sin(x/180*PI)+1);
     g=127*(sin(x/180*PI+3/2*PI)+1);
     b=127*(sin(x/180*PI+0.5*PI)+1);
    
  //convert flaot r,g,b to integers that can be assigned to LED PWM numbers
  
     LED1= int(r);
     LED2= int(g);
     LED3= int(b);
    
  //write LED levels to p0, p1, p4 (ASSIGN PWM values to LEDs)
  
     analogWrite (p0,LED1);
     analogWrite (p1,LED2);
     analogWrite (p4,LED3);
    
  //wait for 1/100 of a second
  
      delay(100);
  }
}

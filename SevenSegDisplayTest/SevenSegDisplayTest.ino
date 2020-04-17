// Display the 15 digits 0-F on a seven-segment display.
// This sketch will eventually be the basis for a sketch to be used on
// ATTiny25 microcontrollers to create the behavior of BCD-to-Seven-segment
// display drivers.
//
// Roger Banks
// 2020 04 15
//
// FND503 Common Cathod Seven Segment Display
//
//       1
//       0  9  G  4  5     <-- Arduino pins
//       |  |  |  |  |
//       g  f     a  b
//     +---------------+
//     |       a       |F
//     |      ---      |N
//     |    f| g |b    |D
//     |      ---      |5
//     |    e|   |c    |0
//     |      ---      |3
//     |       d       |
//     +---------------+
//       e  d     d
//       |  |  |  |  |
//       8  6  G  7  N    <-- Arduino pins
//                   C
//
// ATTiny24/44/84 Microcontroller
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//             (D  0)  PB0  2|    |13  AREF (D 10)
//             (D  1)  PB1  3|    |12  PA1  (D  9) 
//                     PB3  4|    |11  PA2  (D  8) 
//  PWM  INT0  (D  2)  PB2  5|    |10  PA3  (D  7) 
//  PWM        (D  3)  PA7  6|    |9   PA4  (D  6) 
//  PWM        (D  4)  PA6  7|    |8   PA5  (D  5)        PWM
//                           +—--—+

// Array of bitmasks encoding indivudual segements needed on the dispaly to represent the number at the array index
byte numbers[] = {B11111100,  //0
                  B01100000,  //1
                  B11011010,  //2
                  B11110010,  //3
                  B01100110,  //4
                  B10110110,  //5
                  B10111110,  //6
                  B11100000,  //7
                  B11111110,  //8
                  B11100110,  //9
                  B11101110,  //A
                  B00111110,  //B
                  B10011100,  //C
                  B01111010,  //D
                  B10011110,  //E
                  B10001110}; //F

// This function takes a number as it's input and then turns on the appropriate pins to represent
// that number on the display.
// A better programmer would do this with a loop and some bit shifting.
byte output_number(byte n) {

  byte num = numbers[n];
  digitalWrite( 4, B10000000 & num);
  digitalWrite( 5, B01000000 & num);
  digitalWrite( 6, B00100000 & num); 
  digitalWrite( 7, B00010000 & num); 
  digitalWrite( 8, B00001000 & num); 
  digitalWrite( 9, B00000100 & num);
  digitalWrite(10, B00000010 & num);
}

void setup() {

  // Setup pins 4-10 for output
  for (int i = 4; i <= 10; i++) pinMode(i, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (byte i=0; i < 16; i++) {
    output_number(i);
    delay(1000);
  }
}

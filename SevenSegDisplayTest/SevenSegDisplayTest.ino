// Display the 15 digits 0-F on a seven-segment display.
// This sketch will eventually be the basis for a sketch to be used on
// ATTiny24 microcontrollers to create the behavior of BCD-to-Seven-segment
// display drivers.
//
// Roger Banks
// 2020 04 15
//
// 2020 04 17: Code loaded on ATTiny24 and driving display
// 2020 04 18: Code reads input and outputs value to display
//
// TODO:
//        Use direct port manipulation to save program space.
//
// FND503 Common Cathod Seven Segment Display
//
//       
//       6  5  G  0  1     <-- Arduino pins
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
//       e  d     c
//       |  |  |  |  |
//       4  3  G  2  N    <-- Arduino pins
//                   C
//
// ATTiny24/44/84 Microcontroller
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//              (D10)  PB0  2|    |13  PA0  (D0) AREF
//               (D9)  PB1  3|    |12  PA1  (D1) 
//        RST          PB3  4|    |11  PA2  (D2) 
//  PWM  INT0    (D8)  PB2  5|    |10  PA3  (D3) 
//  PWM          (D7)  PA7  6|    |9   PA4  (D4) SCL
//  PWM  MOSI    (D6)  PA6  7|    |8   PA5  (D5) MOSI   PWM
//                           +—--—+

#define PA0  0
#define PA1  1
#define PA2  2
#define PA3  3
#define PA4  4
#define PA5  5
#define PA6  6
#define PA7  7
#define PB2  8
#define PB1  9
#define PB0 10

byte n = 0;

// Array of bitmasks encoding indivudual segements needed on the display to represent the number at the array index
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
  //digitalWrite(PA0, B10000000 & num); //a
  PORTA =  (B10000000 & num) >> 7;
  //digitalWrite(PA1, B01000000 & num); //b
  PORTA |= (B01000000 & num) >> 5;
  //digitalWrite(PA2, B00100000 & num); //c
  PORTA |= (B00100000 & num) >> 3;
  //digitalWrite(PA3, B00010000 & num); //d
  PORTA |= (B00010000 & num) >> 1;
  //digitalWrite(PA4, B00001000 & num); //e
  PORTA |= (B00001000 & num) << 1;
  //digitalWrite(PA5, B00000100 & num); //f
  PORTA |= (B00000100 & num) << 3;
  //digitalWrite(PA6, B00000010 & num); //g
  PORTA |= (B00000010 & num) << 5;
}

// This function reads the values on the input pins and then accumulates
// them as binary numbers into a decimal value. The decimal value is returned.
byte input_number() {
  byte val = 0;                  // value read on pin
  byte n = 0;                    // return value accumulator
  val = digitalRead(PA7);
  if(val) n += 1;
  val = digitalRead(PB2);
  if(val) n += 2;
  val = digitalRead(PB1);
  if(val) n += 4;
  val = digitalRead(PB0);
  if(val) n += 8;
  return n;
}

void setup() {
  // Setup pins 0-6 for output
  //for (int i = 0; i <= 6; i++) pinMode(i, OUTPUT);
  DDRA = 0b01111111;
  // Setup pins 7-10 for input
  //for (int i = 7; i <= 10; i++) pinMode(i, INPUT);
  DDRB = 0b00000000;
}

void loop() {
  n = input_number();
  output_number(n);
}

/*
This code lights up each LED connected to a 74HC595 as determined by the binary value of a counter.
MR to Vcc
OE to GND

                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |    
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |    
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ] |   
         | [ ]IOREF                 MISO/12[ ] |   SH_CP
         | [ ]RST                   MOSI/11[ ]~|   ST_CP
         | [ ]3V3    +---+               10[ ]~|   DS
         | [ ]5v    -| A |-               9[ ]~|   .
         | [ ]GND   -| R |-               8[ ] |   
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[ ] |   D7
         |          -| I |-               6[ ]~|   .
         | [ ]A0    -| N |-               5[ ]~|   .
         | [ ]A1    -| O |-               4[ ] |   .
         | [ ]A2     +---+           INT1/3[ ]~|   .
         | [ ]A3                     INT0/2[ ] |   .
         | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
         | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/
		  
		  http://busyducks.com/ascii-art-arduinos

*/
//Pin connected to SRCLK of 74HC595 
int CLOCK = 12; // Pin connected to RCLK of 74HC595
int LATCH = 11; // Pin connected to SER of 74HC595 
int DATA = 10;

byte counter = 0;

void setup() { 
//set pins to output so you can control the shift register 
pinMode(LATCH, OUTPUT); 
pinMode(CLOCK, OUTPUT); 
pinMode(DATA, OUTPUT); 
}

void loop() { 
// take the latchPin low 
digitalWrite(LATCH, LOW); 
// shift out the bits: 
shiftOut(DATA, CLOCK, MSBFIRST, counter);
//take the latch pin high so the LEDs update: 
digitalWrite(LATCH, HIGH); 
// pause before next value: 
delay(100);

counter = counter + 1;

}

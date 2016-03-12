//     Arduino code to support LED Analyzer Python script
//
//    Copyright (C) 2011  bitrex@earthlink.net
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <Wire.h>

#define MCP4725_ADDRESS 0x60
#define MCP4725_PROGRAM 0x40
#define VOLTAGE_RANGE (4095/5)*3
#define ANALOG_PIN A0

void setup() {

  Serial.begin(9600);
  Wire.begin();

}

void loop() {

  char startByte = '!';
  
  Wire.beginTransmission(MCP4725_ADDRESS);
  Wire.send(MCP4725_PROGRAM);
  Wire.send(0x0);
  Wire.send(0x0);
  Wire.endTransmission();
 
  if (Serial.available() > 0){
    
      if (startByte == Serial.read()){
        Serial.write(startByte);
        Blink();
        runTest();
      }
  }
}
   

void runTest() {

  unsigned int dataValue = 0;
  char endByte = '!';

  for (unsigned int i = 0; i < VOLTAGE_RANGE; i++){

    Wire.beginTransmission(MCP4725_ADDRESS);
    Wire.send(MCP4725_PROGRAM);
    Wire.send((unsigned char)((i & 0xFF0) >> 4));
    Wire.send((unsigned char)((i & 0xF) << 4));
    Wire.endTransmission();

    delay(1);

    dataValue = analogRead(ANALOG_PIN);

    Serial.write((unsigned char)((dataValue & 0x3FC) >> 2)); // high byte
    Serial.write((unsigned char)(dataValue & 0x3)); //low bits

  }
  
  Serial.write(endByte);
  Blink();
  while(1);
  
} 

void Blink() {

  for (unsigned char i = 0; i < 3; i++){

    Wire.beginTransmission(MCP4725_ADDRESS);
    Wire.send(MCP4725_PROGRAM);
    Wire.send(0x7F);
    Wire.send(0x0);
    Wire.endTransmission();
    delay(200); 
    Wire.beginTransmission(MCP4725_ADDRESS);
    Wire.send(MCP4725_PROGRAM);
    Wire.send(0x0);
    Wire.send(0x0);
    Wire.endTransmission();
    delay(200);
  }
}





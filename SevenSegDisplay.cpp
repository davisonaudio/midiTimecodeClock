#include "SevenSegDisplay.h"


void SevenSegDisplay::begin(byte brightness) {
  display1.begin(0x70); //Default i2c address of display driver
  display2.begin(0x71); 
  display1.setBrightness(brightness);
  display2.setBrightness(brightness);
}

void SevenSegDisplay::writeDisplay(int num1, int num2, int num3, int num4) {
//For each digit, 10s are displayed by /10, units displayed by %10.
  
  display1.writeDigitNum(0, num1 / 10, DEC);
  display1.writeDigitNum(1, num1 % 10, DEC);
  
  display1.writeDigitNum(3, num2 / 10, DEC);
  display1.writeDigitNum(4, num2 % 10, DEC);
  
  display2.writeDigitNum(0, num3 / 10, DEC);
  display2.writeDigitNum(1, num3 % 10, DEC);
  
  display2.writeDigitNum(3, num4 / 10, DEC);
  display2.writeDigitNum(4, num4 % 10, DEC);

//Colons
  display1.drawColon(true); //Only draws middle colon of display
  display2.writeDigitRaw(2,0b1110); //Draws both colons on display

//Display changes
  display1.writeDisplay();
  display2.writeDisplay();
  
}

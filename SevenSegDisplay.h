#ifndef SEVENSEGDISPLAY_H
#define SEVENSEGDISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

class SevenSegDisplay
//This class creates two instances of the Adafruit_7segment class, creating an 8 digit display.
//Numbers can be written to this display using 4 separate integers. 0s are shown in LH digit for numbers < 10.

{
  private:
    Adafruit_7segment display1;
    Adafruit_7segment display2;
    
  public:

  //Initialises both displays, sets to default and default+1 i2c addresses, sets brightness
    void begin(byte brightness);

  //Writes four integers to display, left to right. Integers must be <100 (as each uses 2 digits).
    void writeDisplay(int num1, int num2, int num3, int num4);
};

#endif

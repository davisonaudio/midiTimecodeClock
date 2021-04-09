//MIDI Timecode clock
//Designed to display MIDI Timecode (using USB MIDI) or Time of Day on an 8 digit 7 segment display
//Designed using Arduino Pro Micro (ATMega 32u4) for USB HID capabilities.

#include <MIDIUSB.h>
#include <RTClib.h>
#include "MidiTimecode.h"
#include "SevenSegDisplay.h" 

#define LED_BRIGHTNESS 2 //0-15
#define CLOCK_SWITCH_PIN 7


RTC_DS3231 rtc; //Real-Time Clock

MidiTimecode timecode;

SevenSegDisplay timeDisplay;


enum timecodeNumbers{ //Switch state enum (LOW = ToD, HIGH = TC)
    timeOfDayMode,
    timecodeMode
};

volatile bool switchState = LOW; //State of switch to select either time of day or timecode


void updateDisplay(); //Updates the display with either timecode or time of day values, depending on switch state

void switchChanged(); //Interrupt servive routine to handle the toggle switch interrupt

void setup() {
  
  rtc.begin() //Initialise RTC with default i2c address - 0x68
  timeDisplay.begin(LED_BRIGHTNESS); //Initialise displays with default i2c (and default + 1) - 0x70 & 0x71

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Reset RTC to time of compilation
  }
  
  

//Set switch pin, enabling internal pull-up resistor
  pinMode(CLOCK_SWITCH_PIN, INPUT_PULLUP);

//Get initial switch state
  switchState = digitalRead(CLOCK_SWITCH_PIN);

//Attach interrupt to change in value of the toggle switch digital pin
  attachInterrupt(digitalPinToInterrupt(CLOCK_SWITCH_PIN), switchChanged, CHANGE);
}

void loop() {
  
  midiEventPacket_t incomingMIDI = MidiUSB.read();
  
  if (incomingMIDI.header != 0) { //Check if new MIDI message
    
  //Write new messages to timecode buffer (USB MIDI bundled in 3 byte packets)
    timecode.writeByte(incomingMIDI.byte1);
    timecode.writeByte(incomingMIDI.byte2);
    timecode.writeByte(incomingMIDI.byte3);
  }
  
  updateDisplay();
}

void updateDisplay() {
  if (switchState == timecodeMode){
    timeDisplay.writeDisplay(timecode.getHours(), timecode.getMinutes(), timecode.getSeconds(), timecode.getFrames());
  }
  else{
    DateTime now = rtc.now(); //Retrieve current time from RTC
    timeDisplay.writeDisplay(now.hour(), now.minute(), now.second(), 0);
  }
}


void switchChanged(){
  switchState = digitalRead(CLOCK_SWITCH_PIN);
}

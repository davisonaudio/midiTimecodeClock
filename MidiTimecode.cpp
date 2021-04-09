#include "MidiTimecode.h"

//Timecode numbers according to MTC specification
enum timecodeNumbers{
    frameLSnibble,
    frameMSnibble,
    secondLSnibble,
    secondMSnibble,
    minuteLSnibble,
    minuteMSnibble,
    hourLSnibble,
    rateAndHourMSb
};

void MidiTimecode::writeByte(byte midiByte) {

//Write new byte into buffer
  mMIDIBuffer[mWritePoint] = midiByte;

//Check for quarter frame message
  if ( readByte(1) == 0xF1) {
    int timecodeNumber = (midiByte & 0xF0) >> 4; //bitmask & shift to retrieve MS nibble
    int timecodeValue = midiByte & 0x0F; //bitmask to retrieve LS nibble
    processQuarterFrame(timecodeNumber, timecodeValue);
  }

//Check for MTC SysEx Message
  checkSysExTimecode();
  
  //Iterate write point, looping back to start at the end of the buffer.
  mWritePoint = (mWritePoint + 1) % BUFFER_SIZE;
}

void MidiTimecode::processQuarterFrame(int timecodeNumber, int timecodeValue){
  switch(timecodeNumber){
  //If LSnibble, store LSnibble values 
  //If MSnibble, calculate TC value using both nibbles and store.
    
    case frameLSnibble:
      mFrameLSNibble = timecodeValue;
      break;
    
    case frameMSnibble:
      mFrame = mFrameLSNibble + (timecodeValue << 4);
      break;
    
    case secondLSnibble:
      mSecondLSNibble = timecodeValue;
      break;
    
    case secondMSnibble:
      mSecond = mSecondLSNibble + (timecodeValue << 4);
      break;
    
    case minuteLSnibble:
      mMinuteLSNibble = timecodeValue;
      break;
    
    case minuteMSnibble:
      mMinute = mMinuteLSNibble + (timecodeValue << 4);
      break;
    
    case hourLSnibble:
      mHourLSNibble = timecodeValue;
      break;
    
    case rateAndHourMSb:
      mHour = mHourLSNibble + (timecodeValue & 0x01 << 4);
      mFramerate = (timecodeValue & 0x06 >> 1);
      break;
  }
  
}

void MidiTimecode::checkSysExTimecode(){
//Check for MTC SysEx message in buffer
  if (readByte(11) == 0xF0 && readByte(10) == 0x7F && readByte(8) == 0x01 && readByte(7) == 0x01 && readByte(2) == 0xF7){

  //Parse and store timecode values
    mHour = (readByte(6) & 0x1F);
    mMinute = readByte(5);
    mSecond = readByte(4);
    mFrame = readByte(3);
  }
}


byte MidiTimecode::readByte(int n){
//Returns byte value n number of bytes behind write point
  int i = ((mWritePoint + BUFFER_SIZE - n) % BUFFER_SIZE);
  return mMIDIBuffer[i];
}

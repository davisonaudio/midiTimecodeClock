#ifndef MIDITIMECODE_H
#define MIDITIMECODE_H

#include <Arduino.h>

#define BUFFER_SIZE 12 //Must be at least 12 for quarter frame & SysEx messages, at least 2 for just QF messages

class MidiTimecode
//Class to parse MIDI timecode from incoming MIDI data. Uses buffer of MIDI bytes and parse both quarter frame and SysEx MTC messages
{
  private:
  //These variables store the current TC values
    int mHour = 0;
    int mMinute = 0;
    int mSecond = 0;
    int mFrame = 0;
    int mFramerate = 0;

  //Least sig. nibbles are stored as TC values are only updated on receipt of MSnibble, as MTC QF messages are little-endian
    int mHourLSNibble;
    int mMinuteLSNibble;
    int mSecondLSNibble;
    int mFrameLSNibble;
    int mFramerateLSNibble;

  //Current write point in buffer.
    int mWritePoint = 0;

  //Circular buffer of MIDI bytes, written to by writeByte().
    byte mMIDIBuffer[BUFFER_SIZE];

  //Processes quarter frame message after the TC number and value are parsed.
  //Stores the LSnibble of each value, calculates TC value when MSnibble is received
    void processQuarterFrame(int timecodeNumber, int timecodeValue);

  //Checks for an MTC SysEx message, parsing the MTC values if message found.
    void checkSysExTimecode();

  protected:
  //Returns byte value in MIDI buffer n number of bytes behind the write point
    byte readByte(int n);
    
    
  public:

  //Writes a single MIDI byte into the buffer
  //MTC messages are checked for during this function and values updated appropriately.
    void writeByte(byte midiByte);

  //Accessor functions
    int getHours(){return mHour;};
    int getMinutes(){return mMinute;};
    int getSeconds(){return mSecond;};
    int getFrames(){return mFrame;};

};

#endif

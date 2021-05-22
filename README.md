# midiTimecodeClock
MIDI timecode parser library with Arduino program to display timecode or time of day (using an RTC) on an 8 digit 7 segment display

The provided Arduino sketch uses USB MIDI with an Arduino Micro board, though this could be altered easily to use a Serial UART connection for traditional MIDI input.

## Future additions:

- Add ability to set RTC time with a MIDI SysEx message.
- Add a timeout feature to turn off display a certain amount of time after last receiving a MIDI message.

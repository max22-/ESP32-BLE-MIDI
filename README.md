ESP32-BLE-MIDI
==============

A small Arduino library to use Midi over Bluetooth Low Energy (BLE) on ESP32 boards.
Work in progress, there will be a release soon.

Quick start
-----------

```
#include <BLEMidi.h>

BLEMidi bleMidi("My awesome MIDI device");

void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  Serial.print("Received note on : channel ");
  Serial.print(channel);
  Serial.print(", note ");
  Serial.print(note);
  Serial.print(", velocity ");
  Serial.println(velocity);
}

void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  Serial.print("Received note off : channel ");
  Serial.print(channel);
  Serial.print(", note ");
  Serial.print(note);
  Serial.print(", velocity ");
  Serial.println(velocity);
}

void setup() {
  Serial.begin(115200);
  bleMidi.begin();
  bleMidi.setNoteOnCallback(onNoteOn);
  bleMidi.setNoteOffCallback(onNoteOff);
}

void loop() {
  if (bleMidi.isConnected()) {
    bleMidi.noteOn(0, 60, 127);
    delay(1000);
    bleMidi.noteOff(0, 60, 0);
    delay(1000);
    bleMidi.controlChange(0, 45, 53);
    delay(1000);
    bleMidi.programChange(0, 53);
    delay(1000);
  }
}
```

Future work
-----------

- Implement missing features (aftertouch, etc).
- Add some more examples
- Add documentation, with Doxygen ?

- Add support for realtime messages ?
- Add debugging messages in BLEMidiServer ?





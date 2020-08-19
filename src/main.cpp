/*
Code based on :
https://github.com/neilbags/arduino-esp32-BLE-MIDI/blob/master/BLE_MIDI.ino (write midi messages)
https://gist.github.com/johnty/cbfa66c3369a692410f3493c20b2b3e2 (read and write)
https://circuitdigest.com/microcontroller-projects/esp32-ble-client-connecting-to-fitness-band-to-trigger-light (initiate the connection to another bluetooth device)

*/

#include <Arduino.h>
#include <BLEMidi.h>

BLEMidi bleMidi("Maxime's MIDI device");

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
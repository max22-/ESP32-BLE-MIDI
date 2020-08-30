/*
Code based on :
https://github.com/neilbags/arduino-esp32-BLE-MIDI/blob/master/BLE_MIDI.ino (write midi messages)
https://gist.github.com/johnty/cbfa66c3369a692410f3493c20b2b3e2 (read and write)
https://circuitdigest.com/microcontroller-projects/esp32-ble-client-connecting-to-fitness-band-to-trigger-light (initiate the connection to another bluetooth device)
and Arduino BLE_Client example sketch for ESP32 boards
*/

#include <Arduino.h>
#include <BLEMidi.h>

void connected();
void disconnected();

BLEMidiClient bleMidi("Maxime's MIDI device", connected, disconnected);

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

void onControlChange(uint8_t channel, uint8_t controller, uint8_t value)
{
  if(controller == 125)
    Serial.printf("Volume : %d\n", value*100/127);
}

void connected()
{
  Serial.println("Connected");
}

void disconnected()
{
  Serial.println("Disconnected");
}

void setup() {
  Serial.begin(115200);
  bleMidi.begin();
  bleMidi.setNoteOnCallback(onNoteOn);
  bleMidi.setNoteOffCallback(onNoteOff);
  bleMidi.setControlChangeCallback(onControlChange);
}

void loop() {

  if(bleMidi.isConnected()) {
    Serial.println("Sending drum on command");
    bleMidi.controlChange(0, 122, 127);
    bleMidi.controlChange(0, 125, 50*127/100);
  }
  else {
    int nDevices = bleMidi.scan();
    if(nDevices > 0) {
      if(bleMidi.connect(0))
        Serial.println("Connection established");
      else
        Serial.println("Connection failed");
    }
  }
  delay(1000);
  /*if (bleMidi.isConnected()) {
    bleMidi.noteOn(0, 60, 127);
    delay(1000);
    bleMidi.noteOff(0, 60, 0);
    delay(1000);
    bleMidi.controlChange(0, 45, 53);
    delay(1000);
    bleMidi.programChange(0, 53);
    delay(1000);
  }
  */
}
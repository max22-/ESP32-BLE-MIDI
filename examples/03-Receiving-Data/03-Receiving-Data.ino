#include <Arduino.h>
#include <BLEMidi.h>

void connected();
void disconnected();

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
    Serial.printf("Received control change : channel %d, controller %d, value %d\n", channel, controller, value);
}

void connected()
{
  Serial.println("Connected");
}

void setup() {
  Serial.begin(115200);
  BLEMidiServer.begin("MIDI device");
  BLEMidiServer.setOnConnectCallback(connected);
  BLEMidiServer.setOnDisconnectCallback([](){     // To show how to make a callback with a lambda function
    Serial.println("Disconnected");
  });
  BLEMidiServer.setNoteOnCallback(onNoteOn);
  BLEMidiServer.setNoteOffCallback(onNoteOff);
  BLEMidiServer.setControlChangeCallback(onControlChange);
  //BLEMidiServer.enableDebugging();
}

void loop() {
  if (BLEMidiServer.isConnected()) {
      BLEMidiServer.noteOn(0, 69, 127);
      delay(1000);
      BLEMidiServer.noteOff(0, 69, 127);
      delay(1000);
  }
  delay(1);
}
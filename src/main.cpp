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
  BLEMidiClient.begin("Maxime's MIDI device");
  BLEMidiClient.setOnConnectCallback(connected);
  BLEMidiClient.setOnDisconnectCallback(disconnected);
  BLEMidiClient.setNoteOnCallback(onNoteOn);
  BLEMidiClient.setNoteOffCallback(onNoteOff);
  BLEMidiClient.setControlChangeCallback(onControlChange);
  //BLEMidiClient.enableDebugging();
}

void loop() {

  if(BLEMidiClient.isConnected()) {
    Serial.println("Sending drum on command");
    BLEMidiClient.controlChange(0, 122, 127);
    BLEMidiClient.controlChange(0, 125, 50*127/100);
  }
  else {
    int nDevices = BLEMidiClient.scan();
    if(nDevices > 0) {
      if(BLEMidiClient.connect())
        Serial.println("Connection established");
      else
        Serial.println("Connection failed");
    }
  }
  delay(1000);
}
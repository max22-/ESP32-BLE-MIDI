#include <Arduino.h>
#include <BLEMidi.h>

// Configuration
const int pot_pin = 1;
const uint8_t midi_channel = 0;
const uint8_t controller_number = 0;
const float c = 0.2;  // low pass filter coefficient (0 < c <= 1)


void setup() {
  Serial.begin(115200);
  pinMode(pot_pin, INPUT);
  BLEMidiServer.begin("Midi knob");
}

void loop() {
  static float y = 0;
  static uint8_t old_y = 0;

  float x = map(analogRead(pot_pin), 0, 4095, 0, 127);
  y += c * (x - y);  // simple low pass IIR filter (see tutorial here : https://tomroelandts.com/articles/low-pass-single-pole-iir-filter )
                     // (when we read the potentiometer, the value flickers a little bit all the time, so we need to do filter it to avoid sending midi messages all the time)
                     // another solution may be better than this type of filter, it is left as an exercise to the reader :)  
  if(BLEMidiServer.isConnected() && (uint8_t)y != old_y) {
    BLEMidiServer.controlChange(midi_channel, controller_number, y);
    old_y = (uint8_t)y;
  }
}
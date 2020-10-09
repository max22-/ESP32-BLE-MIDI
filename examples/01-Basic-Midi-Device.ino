#include <Arduino.h>
#include <BLEMidi.h>

BLEMidiServer bleMidi("Basic MIDI device");

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing bluetooth");
  bleMidi.begin();
  Serial.println("Waiting for connections...");
  //bleMidi.enableDebugging();  // Uncomment if you want to see some debugging output from the library
}

void loop() {
  if(bleMidi.isConnected()) {             // If we've got a connection, we send an A4 during one second, at full elocity (127)
      bleMidi.noteOn(0, 69, 127);
      delay(1000);
      bleMidi.noteOff(0, 69, 127);        // Then we make a delay of one second before returning to the beginning of the loop
      delay(1000);
  }
  delay(1);   // we feed the ESP32 watchdog when there is no connection
}
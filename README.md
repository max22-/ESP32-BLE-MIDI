ESP32-BLE-MIDI
==============

A small Arduino library to use Midi over Bluetooth Low Energy (BLE) on ESP32 boards.
Work in progress, there will be a release soon.

Quick start
-----------

```
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
  if(bleMidi.isConnected()) {             // If we've got a connection, we send an A4 during one second, at full velocity (127)
      bleMidi.noteOn(0, 69, 127);
      delay(1000);
      bleMidi.noteOff(0, 69, 127);        // Then we stop the note and make a delay of one second before returning to the beginning of the loop
      delay(1000);
  }
  delay(1);   // we feed the ESP32 watchdog when there is no connection
}
```

Future work
-----------

- Implement missing features (aftertouch, etc).
- Add some more examples
- Add documentation, with Doxygen ?

- Add support for realtime messages ?
- Add debugging messages in BLEMidiServer ?

Thanks
------
Thanks to the authors of those pages / pieces of code :
- https://github.com/neilbags/arduino-esp32-BLE-MIDI/blob/master/BLE_MIDI.ino (write midi messages)
- https://gist.github.com/johnty/cbfa66c3369a692410f3493c20b2b3e2 (read and write)
- https://circuitdigest.com/microcontroller-projects/esp32-ble-client-connecting-to-fitness-band-to-trigger-light (initiate the connection to another bluetooth device)
- and the Arduino BLE_Client example sketch for ESP32 boards


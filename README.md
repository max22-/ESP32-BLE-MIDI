ESP32-BLE-MIDI
==============

An Arduino library to use Midi over Bluetooth Low Energy (BLE) on ESP32 boards.  
If you find it useful you can <a href='https://ko-fi.com/C0C7NIYDY' target='_blank'><img height='36' style='border:0px;height:36px;' src='https://storage.ko-fi.com/cdn/kofi2.png?v=3' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a> if you want.

Quick start
-----------

```
#include <Arduino.h>
#include <BLEMidi.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing bluetooth");
  BLEMidiServer.begin("Basic MIDI device");
  Serial.println("Waiting for connections...");
  //BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library
}

void loop() {
  if(BLEMidiServer.isConnected()) {             // If we've got a connection, we send an A4 during one second, at full velocity (127)
      BLEMidiServer.noteOn(0, 69, 127);
      delay(1000);
      BLEMidiServer.noteOff(0, 69, 127);        // Then we stop the note and make a delay of one second before returning to the beginning of the loop
      delay(1000);
  }
}
```

Check the header file [here](https://github.com/max22-/ESP32-BLE-MIDI/blob/master/src/utility/Midi.h) to view all available MIDI commands and callbacks.

Changes
-------

  - v0.3.2
    - 2023-04-25
      - Added BLEMidi::end() to stop the BLE client or server.
      
  - v0.3.1
    - 2023-03-12
      - Added a simple knob example

  - v0.3.0
    - 2022-12-27
      - Migrated to NimBLE using [NimBLE-Arduino library](https://github.com/h2zero/NimBLE-Arduino)

  - 2022-01-23
    - Merged PR from [trdenton](https://github.com/trdenton) : Support for [MMC transport controls](https://en.wikipedia.org/wiki/MIDI_Machine_Control)

- v0.2.2
  - 2021-09-20
    - Bug corrections : connection/disconnection callbacks weren't called when they where set up after BLEMidiServer.begin(),
    and no more connection were accepted after a disconnection. Thanks to [Kim](https://github.com/buzz-dk) for the information provided.

- v0.2.1
  - 2021-03-16
    - Bug correction : the WDT used to reset the ESP32 when lots of messages were received, because the IDLE task didn't have time to call esp_task_wdt_reset

- v0.2.0
  - 2021-03-12
    - Implemented support for running status messages
  - 2021-03-13
    - After touch support

- v0.1.2
  - 2021-03-02 
    - Added pitch bend callback
    - Added a range parameter for pitch bend sending
  - 2021-03-11
    - Added an a new overload for pitch bend sending

- v0.1.1
  - 2020-12-29
    - Integrated bug fix from [Georaura](https://github.com/georaura)
  - 2020-12-30
    - Implemented packet timestamps

Future work
-----------

- Add some more examples
- Add documentation, with Doxygen ?
- Add keywords.txt for Arduino IDE

- Add support for realtime messages ?
- Add debugging messages in BLEMidiServer ?
- Better debug function

Thanks
------
Thanks to the authors of those pages / pieces of code :
- https://github.com/neilbags/arduino-esp32-BLE-MIDI/blob/master/BLE_MIDI.ino (write midi messages)
- https://gist.github.com/johnty/cbfa66c3369a692410f3493c20b2b3e2 (read and write)
- https://circuitdigest.com/microcontroller-projects/esp32-ble-client-connecting-to-fitness-band-to-trigger-light (initiate the connection to another bluetooth device)
- and the Arduino BLE_Client example sketch for ESP32 boards

Message to users
----------------
If you make some noise with it, I would be glad to see your projects ! Don't hesitate to drop me an e-mail.
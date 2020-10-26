ESP32-BLE-MIDI
==============

An Arduino library to use Midi over Bluetooth Low Energy (BLE) on ESP32 boards.

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
  delay(1);   // we feed the ESP32 watchdog when there is no connection
}
```

Future work
-----------

- Implement missing features (aftertouch, etc).
- Add some more examples
- Add documentation, with Doxygen ?
- Add keywords.txt for Arduino IDE

- Add support for realtime messages ?
- Add debugging messages in BLEMidiServer ?

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


Donations
---------
I work on this project for pleasure, but if you think it is worth some money, feel free to send me the amount you want, via Paypal or Bitcoin.

| PayPal | Bitcoin |
| ------ | ------- |
| [![](https://www.paypalobjects.com/en_US/FR/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=A6ATX3NY4DFEQ) |  <center> [![](http://api.qrserver.com/v1/create-qr-code/?color=000000&bgcolor=FFFFFF&data=bitcoin%3A1AM3HCBRrXQcQMFckB1LH1VZvxbecvVxJd&qzone=1&margin=0&size=200x200&ecc=L)](bitcoin:1AM3HCBRrXQcQMFckB1LH1VZvxbecvVxJd)<br />[1AM3HCBRrXQcQMFckB1LH1VZvxbecvVxJd](bitcoin:1AM3HCBRrXQcQMFckB1LH1VZvxbecvVxJd)</center> |

#include <Arduino.h>
#include <BLEMidi.h>

BLEMidiClient bleMidi("Midi client");

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing bluetooth");
    bleMidi.begin();

    //bleMidi.enableDebugging();  // Uncomment to see debugging messages from the library

}

void loop() {
    if(!bleMidi.isConnected()) {
        // If we are not already connected, we try te connect to the first BLE Midi device we find
        int nDevices = bleMidi.scan();
        if(nDevices > 0) {
            if(bleMidi.connect(0))
                Serial.println("Connection established");
            else {
                Serial.println("Connection failed");
                delay(3000);    // We wait 3s before attempting a new connection
            }
        }
    }
    else {
        Serial.println("Sending drum on command");
        // Real world example : it starts the drum function of the NUX Mighty Lit BT guitar amplifier
        // https://www.nuxefx.com/mighty-lite-bt.html
        // This is the only bluetooth controllable device that I have.
        bleMidi.controlChange(0, 122, 127);
        bleMidi.controlChange(0, 125, 50*127/100);
    }
}
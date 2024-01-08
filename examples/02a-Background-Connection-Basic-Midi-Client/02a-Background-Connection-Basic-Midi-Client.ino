#include <BLEMidi.h>

#include <Arduino.h>
#include <BLEMidi.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing bluetooth");
    BLEMidiClient.begin("Midi client"); // "Midi client" is the name you want to give to the ESP32

    BLEMidiClient.enableDebugging();  // Uncomment to see debugging messages from the library
    
    BLEMidiClient.backgroundScan();
}

void loop() {
    if(BLEMidiClient.isConnected()) {
        Serial.println("Sending drum on command");
        // Real world example : it starts the drum function of the NUX Mighty Lit BT guitar amplifier
        // https://www.nuxefx.com/mighty-lite-bt.html
        // This is the only bluetooth controllable device that I have.
        BLEMidiClient.controlChange(0, 122, 127);
        BLEMidiClient.controlChange(0, 125, 50*127/100);
        delay(5000);
    }
}
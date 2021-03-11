#include <Arduino.h>
#include <BLEMidi.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing bluetooth");
    BLEMidiClient.begin("Midi client"); // "Midi client" is the name you want to give to the ESP32

    //BLEMidiClient.enableDebugging();  // Uncomment to see debugging messages from the library

}

void loop() {
    if(!BLEMidiClient.isConnected()) {
        // If we are not already connected, we try te connect to the first BLE Midi device we find
        int nDevices = BLEMidiClient.scan();
        if(nDevices > 0) {
            if(BLEMidiClient.connect(0))
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
        BLEMidiClient.controlChange(0, 122, 127);
        BLEMidiClient.controlChange(0, 125, 50*127/100);
        delay(5000);
    }
}
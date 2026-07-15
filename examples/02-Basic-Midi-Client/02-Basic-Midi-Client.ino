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
        Serial.println("Scanning for BLE devices...");
        unsigned int nDevices = BLEMidiClient.scan();
        Serial.printf("found %u device(s)\n", nDevices);
        for(unsigned int i = 0; i < nDevices; i++) {
            String name = BLEMidiClient.deviceName(i);
            Serial.printf("- \"%s\" : mac address = %s\n", name.c_str(), BLEMidiClient.deviceMacAddress(i).c_str());
            if(BLEMidiClient.deviceName(i) == "NUX MIGHTY LITE MIDI") {
                if(BLEMidiClient.connect(i)) {
                    Serial.println("Connection established");
                    break; // we break out of the for loop
                } else {
                    Serial.println("Connection failed");
                
                }
            }
        }
    }
    else {
        Serial.println("Sending drum on command");
        // Real world example : it starts the drum function of the NUX Mighty Lite BT guitar amplifier
        // https://www.nuxefx.com/mighty-lite-bt.html
        BLEMidiClient.controlChange(0, 122, 127);
        BLEMidiClient.controlChange(0, 125, 50*127/100);
        delay(5000);
    }
    delay(1000);
}
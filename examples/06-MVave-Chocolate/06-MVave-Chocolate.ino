#include <Arduino.h>
#include <BLEMidi.h>

uint8_t led_state = 0;

void program_change(uint8_t channel, uint8_t program, uint16_t timestamp) {
    Serial.printf("program change: channel=%u, program=%u\n", channel, program);
    if(channel == 0 && program == 0) {
        led_state = !led_state;
        digitalWrite(LED_BUILTIN, led_state);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Initializing bluetooth");
    BLEMidiClient.begin("Midi client"); // "Midi client" is the name you want to give to the ESP32
    BLEMidiClient.setProgramChangeCallback(program_change);

    // BLEMidiClient.enableDebugging();  // Uncomment to see debugging messages from the library

}

void loop() {
    if(!BLEMidiClient.isConnected()) {
        Serial.println("Scanning for BLE devices...");
        unsigned int nDevices = BLEMidiClient.scan();
        for(unsigned int i = 0; i < nDevices; i++) {
            if(BLEMidiClient.deviceName(i) == "FootCtrl") {
                if(BLEMidiClient.connect(i)) {
                    Serial.println("Connection established");
                    break; // we break out of the for loop
                } else {
                    Serial.println("Connection failed");
                
                }
            }
        }
    }
    delay(1000);
}
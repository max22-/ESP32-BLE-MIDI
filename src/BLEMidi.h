#ifndef BLEMIDI_H
#define BLEMIDI_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

class BLEMidi {
public:
    BLEMidi(std::string deviceName);
    void begin();
    bool isConnected();
    void sendPacket(uint8_t *packet, uint8_t packetSize);

    void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void controlChange(uint8_t channel, uint8_t controller, uint8_t value);
    void programChange(uint8_t channel, uint8_t program);
    //void pitchBend();

private:
    std::string deviceName;
    BLECharacteristic *pCharacteristic;
    bool deviceConnected = false;
    std::string SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
    std::string CHARACTERISTIC_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3";
};

class MyServerCallbacks: public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) {
      *deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      *deviceConnected = false;
    }

public:
    MyServerCallbacks(bool *deviceConnected) : deviceConnected(deviceConnected) {}

private:
    bool *deviceConnected;
};

#endif
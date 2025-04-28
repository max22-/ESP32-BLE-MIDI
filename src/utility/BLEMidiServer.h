#ifndef BLE_MIDI_SERVER_H
#define BLE_MIDI_SERVER_H

#include "BLEMidiBase.h"


class BLEMidiServerClass : public BLEMidi, public NimBLEServerCallbacks {
public:
    void begin(const std::string deviceName);

    void setOnConnectCallback(void (*const onConnectCallback)());
    void setOnDisconnectCallback(void (*const onDisconnectCallback)());
    
    

private:
    virtual void sendPacket(uint8_t *packet, uint8_t packetSize) override;
    void onConnect(NimBLEServer* pServer) override;
    void onDisconnect(NimBLEServer* pServer) override;
    
    void (*onConnectCallback)() = nullptr;
    void (*onDisconnectCallback)() = nullptr;
    NimBLECharacteristic* pCharacteristic = nullptr;
};


class CharacteristicCallback: public NimBLECharacteristicCallbacks {
public:
    CharacteristicCallback(std::function<void(uint8_t*, uint8_t)> onWriteCallback);
private:
    void onWrite(NimBLECharacteristic *pCharacteristic);
    std::function<void(uint8_t*, uint8_t)> onWriteCallback = nullptr;
};


extern BLEMidiServerClass BLEMidiServer;

#endif
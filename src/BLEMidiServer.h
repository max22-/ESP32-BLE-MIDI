#ifndef BLE_MIDI_SERVER_H
#define BLE_MIDI_SERVER_H

#include "BLEMidiBase.h"

class BLEMidiServerClass : public BLEMidi {
public:
    void begin(const std::string deviceName);

    void setOnConnectCallback(void (*const onConnectCallback)());
    void setOnDisconnectCallback(void (*const onDisconnectCallback)());
    

private:
    virtual void sendPacket(uint8_t *packet, uint8_t packetSize) override;
    
    void (*onConnectCallback)() = nullptr;
    void (*onDisconnectCallback)() = nullptr;
    BLECharacteristic* pCharacteristic;
};


class MyServerCallbacks: public BLEServerCallbacks {

    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);

public:
    MyServerCallbacks(
        bool *connected, 
        void (*const onConnectCallback)(),
        void (*const onDisconnectCallback)()
    );

private:
    bool *connected;
    void (*const onConnectCallback)();
    void (*const onDisconnectCallback)();
    BLECharacteristic *pCharacteristic;
};


class CharacteristicCallback: public BLECharacteristicCallbacks {
public:
    CharacteristicCallback(std::function<void(uint8_t*, uint8_t)> onWriteCallback);
private:
    void onWrite(BLECharacteristic *pCharacteristic);
    std::function<void(uint8_t*, uint8_t)> onWriteCallback = nullptr;
};


extern BLEMidiServerClass BLEMidiServer;

#endif
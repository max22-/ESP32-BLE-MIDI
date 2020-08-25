#ifndef BLE_MIDI_CLIENT_H
#define BLE_MIDI_CLIENT_H

#include "BLEMidiBase.h"

class BLEMidiClient : public BLEMidi {
public:
    BLEMidiClient(
        const std::string deviceName, 
        void (*const onConnectCallback)() = nullptr,
        void (*const onDisconnectCallback)() = nullptr
    );
    void begin();
    int scan();
    bool connect(int deviceIndex);

private:
    virtual void sendPacket(uint8_t *packet, uint8_t packetSize) override;

    BLEScan *pBLEScan = nullptr;
    BLEScanResults foundDevices;
    BLERemoteCharacteristic* pRemoteCharacteristic;
    void (*const onConnectCallback)();
    void (*const onDisconnectCallback)();
};

class ClientCallbacks : public BLEClientCallbacks {
public:
    ClientCallbacks(
            bool& connected,
            void (* const onConnectCallback)() = nullptr, 
            void (* const onDisconnectCallback)() = nullptr
    );
private:
    void onConnect(BLEClient*);
    void onDisconnect(BLEClient*);

    bool& connected;
    void (* const onConnectCallback)() = nullptr;
    void (* const onDisconnectCallback)() = nullptr;
};

#endif
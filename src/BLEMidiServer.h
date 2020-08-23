#ifndef BLE_MIDI_SERVER_H
#define BLE_MIDI_SERVER_H

#include "BLEMidiBase.h"

class BLEMidiServer : public BLEMidi {
public:
    BLEMidiServer(
        const std::string deviceName, 
        void (*const onConnectCallback)(BLEServer*) = nullptr,
        void (*const onDisconnectCallback)(BLEServer*) = nullptr
    );
    void begin();

private:
    void (*const onConnectCallback)(BLEServer* pServer);
    void (*const onDisconnectCallback)(BLEServer* pServer);
};


class MyServerCallbacks: public BLEServerCallbacks {

    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);

public:
    MyServerCallbacks(
        bool *connected, 
        void (*const onConnectCallback)(BLEServer*),
        void (*const onDisconnectCallback)(BLEServer*)
    );

private:
    bool *connected;
    void (*const onConnectCallback)(BLEServer* pServer);
    void (*const onDisconnectCallback)(BLEServer* pServer);
};


#endif
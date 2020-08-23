#ifndef BLE_MIDI_SERVER_H
#define BLE_MIDI_SERVER_H

#include "BLEMidiBase.h"

class BLEMidiServer : public BLEMidi {
public:
    BLEMidiServer(
        const std::string deviceName, 
        const void (*onConnectCallback)(BLEServer*) = nullptr,
        const void (*onDisconnectCallback)(BLEServer*) = nullptr
    );
    void begin();

private:
    const void (*onConnectCallback)(BLEServer* pServer);
    const void (*onDisconnectCallback)(BLEServer* pServer);
};


class MyServerCallbacks: public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) {
      *connected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      *connected = false;
    }

public:
    MyServerCallbacks(bool *connected) : connected(connected) {}

private:
    bool *connected;
};


#endif
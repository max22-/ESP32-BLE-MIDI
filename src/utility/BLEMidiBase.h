#ifndef BLE_MIDI_BASE_H
#define BLE_MIDI_BASE_H

#include <NimBLEDevice.h>
#include <functional>
#include "Midi.h"

class BLEMidi : public Midi {
public:
    virtual void begin(const std::string deviceName);
    void end();
    bool isConnected();

protected:
    std::string deviceName;
    bool connected = false;

    static const std::string MIDI_SERVICE_UUID;
    static const std::string MIDI_CHARACTERISTIC_UUID;
};


#endif
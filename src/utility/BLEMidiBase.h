#ifndef BLE_MIDI_BASE_H
#define BLE_MIDI_BASE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <functional>
#include "Midi.h"

class BLEMidi : public Midi {
public:
    virtual void begin(const std::string deviceName);
    bool isConnected();

protected:
    std::string deviceName;
    bool connected = false;
    const std::string MIDI_SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
    const std::string MIDI_CHARACTERISTIC_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3";
};


#endif
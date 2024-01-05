#include <Arduino.h>
#include "BLEMidiBase.h"

void BLEMidi::begin(const std::string deviceName)
{
    this->deviceName = deviceName;
    BLEDevice::init(deviceName);
}

void BLEMidi::end() {
    BLEDevice::deinit();
}

bool BLEMidi::isConnected()
{
    return connected;
}

const std::string BLEMidi::MIDI_SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
const std::string BLEMidi::MIDI_CHARACTERISTIC_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3";


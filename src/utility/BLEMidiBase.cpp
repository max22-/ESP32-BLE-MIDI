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
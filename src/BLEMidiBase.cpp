#include <Arduino.h>
#include <BLEMidiBase.h>

BLEMidi::BLEMidi(const std::string deviceName) : deviceName(deviceName)
{

}

void BLEMidi::begin()
{
    BLEDevice::init(deviceName);
}

bool BLEMidi::isConnected()
{
    return connected;
}
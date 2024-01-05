#ifndef ADVERTISED_DEVICE_CALLBACKS_H
#define ADVERTISED_DEVICE_CALLBACKS_H

#include "BLEMidiBase.h"
#include "BLEMidiClient.h"

class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    private:
    std::string m_name;
    std::string m_address;
    BLEMidiClientClass* p_midiClient;
    BLEAdvertisedDevice *foundDevice = nullptr;


    public:
    AdvertisedDeviceCallbacks(std::string const& name, std::string const& address, BLEMidiClientClass* midiclient) {
        m_name = name;
        m_address = address;
        p_midiClient = midiclient;
    }
    void setName (std::string name) {
        m_name = name;
    }
    void setAddress (std::string address) {
        m_address = address;
    }
    BLEAdvertisedDevice* getDevice() {
        return foundDevice;
    }

    void onResult(BLEAdvertisedDevice *advertisedDevice)
    {
    p_midiClient->debug.print("BLE Device found: ");
    if (advertisedDevice->haveServiceUUID() && advertisedDevice->isAdvertisingService(BLEUUID(BLEMidiClientClass::MIDI_SERVICE_UUID)))
    {
        p_midiClient->debug.println("is MIDI device...");
        advertisedDevice->getScan()->stop();
        auto deviceStr = "name = \"" + advertisedDevice->getName() + "\", address = " + advertisedDevice->getAddress().toString();
        p_midiClient->debug.println(deviceStr.c_str());

        //** drop out and continue the search if it is not the midi device we were searching for **//
        if (!m_name.empty() && m_name.compare(advertisedDevice->getName()) != 0)
            return;
        if (!m_address.empty() && m_address.compare(advertisedDevice->getAddress().toString()) != 0)
            return;

        p_midiClient->debug.println("Is our device. Trying to connect...");
        /** stop scan before connecting */
        advertisedDevice->getScan()->stop();
        /** Save the device reference in a global for the client to use*/
        foundDevice = advertisedDevice;
        // p_midiClient->connect(advertisedDevice); //Cannot connect in the callback.
    }
    };

};

#endif
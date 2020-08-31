#include "BLEMidiClient.h"
#include <Arduino.h>            // to remove when serial prints will be removed

BLEMidiClient::BLEMidiClient(
    const std::string deviceName,
    void (*const onConnectCallback)(),
    void (*const onDisconnectCallback)()
    ) 
    :   BLEMidi(deviceName),
        onConnectCallback(onConnectCallback),
        onDisconnectCallback(onDisconnectCallback)
{}

void BLEMidiClient::begin()
{
    BLEMidi::begin();
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
}

int BLEMidiClient::scan()
{
    if(pBLEScan == nullptr)
        return 0;
    pBLEScan->clearResults();
    foundMidiDevices.clear();
    BLEScanResults foundDevices = pBLEScan->start(3);
    Serial.printf("Found %d device(s)\n", foundDevices.getCount());
    for(int i=0; i<foundDevices.getCount(); i++) {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        Serial.println(device.toString().c_str());
        if(device.getServiceUUID().equals(BLEUUID(MIDI_SERVICE_UUID))) {
            Serial.println("It is a MIDI device");
            foundMidiDevices.push_back(device);
        }
        
    }
    return foundMidiDevices.size();
}

BLEAdvertisedDevice* BLEMidiClient::getScannedDevice(uint32_t deviceIndex)
{
    if(deviceIndex >= foundMidiDevices.size())
        return nullptr;
    return &foundMidiDevices.at(deviceIndex);
}

bool BLEMidiClient::connect(uint32_t deviceIndex)
{
    if(deviceIndex >= foundMidiDevices.size())
        return false;
    Serial.printf("getDevice(%d)\n", deviceIndex);
    BLEAdvertisedDevice* device = new BLEAdvertisedDevice(foundMidiDevices.at(deviceIndex));
    Serial.printf("device = 0x%x\n", (void*)device);
    if(device == nullptr)
        return false;
    Serial.printf("Connecting to %s\n", device->getAddress().toString().c_str());
    BLEClient* pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new ClientCallbacks(connected, onConnectCallback, onDisconnectCallback));
    Serial.println("pClient->connect()");
    if(!pClient->connect(device))
        return false;
    Serial.println("pClient->getService()");
    BLERemoteService* pRemoteService = pClient->getService(MIDI_SERVICE_UUID.c_str());
    if(pRemoteService == nullptr)
        return false;
    Serial.println("pRemoteService->getCharacteristic()");
    pRemoteCharacteristic = pRemoteService->getCharacteristic(MIDI_CHARACTERISTIC_UUID.c_str());
    if(pRemoteCharacteristic == nullptr)
        return false;
    if(pRemoteCharacteristic->canNotify())
        CallbackRegister::registerCallback(pRemoteCharacteristic, [this](    // We have to use the CallbackRegister class to be able to call the receivePacket member function as a callback
            BLERemoteCharacteristic* pBLERemoteCharacteristic,          // A little bit overkill... ;)
            uint8_t* pData, 
            size_t length, 
            bool isNotify) {
                receivePacket(pData, length);
        });     
                                                                            
        pRemoteCharacteristic->registerForNotify(&CallbackRegister::mainCallback);
    connected=true;
    return true;
}

void BLEMidiClient::sendPacket(uint8_t *packet, uint8_t packetSize)
{
    if(!connected)
        return;
    pRemoteCharacteristic->writeValue(packet, packetSize);
}

ClientCallbacks::ClientCallbacks(
    bool& connected,
    void (*onConnectCallback)(), 
    void (*onDisconnectCallback)()
) :     connected(connected),
        onConnectCallback(onConnectCallback),
        onDisconnectCallback(onDisconnectCallback)
{}

void ClientCallbacks::onConnect(BLEClient *pClient)
{
    connected = true;
    if(onConnectCallback != nullptr)
        onConnectCallback();
}

void ClientCallbacks::onDisconnect(BLEClient *pClient)
{
    connected = false;
    if(onDisconnectCallback != nullptr)
        onDisconnectCallback();
}

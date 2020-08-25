#include "BLEMidiServer.h"

BLEMidiServer::BLEMidiServer(
    const std::string deviceName,
    void (*const onConnectCallback)(),
    void (*const onDisconnectCallback)()
    ) 
    :   BLEMidi(deviceName), 
        onConnectCallback(onConnectCallback), 
        onDisconnectCallback(onDisconnectCallback)
{}

void BLEMidiServer::begin()
{
    BLEMidi::begin();
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(&connected, onConnectCallback, onDisconnectCallback));
    BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));
    pCharacteristic = pService->createCharacteristic(
        BLEUUID(CHARACTERISTIC_UUID),
        BLECharacteristic::PROPERTY_READ   |
        BLECharacteristic::PROPERTY_WRITE  |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_WRITE_NR
    );
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new CharacteristicCallback(midiCallbacks));
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();
}

void BLEMidiServer::sendPacket(uint8_t *packet, uint8_t packetSize)
{
    if(!connected)
        return;
    pCharacteristic->setValue(packet, packetSize);
    pCharacteristic->notify();
}

MyServerCallbacks::MyServerCallbacks(
    bool *connected, 
    void (*const onConnectCallback)(),
    void (*const onDisconnectCallback)()
) : connected(connected),
    onConnectCallback(onConnectCallback),
    onDisconnectCallback(onDisconnectCallback)
{}

void MyServerCallbacks::onConnect(BLEServer* pServer) {
    *connected = true;
    if(onConnectCallback != nullptr)
        onConnectCallback();
};

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    *connected = false;
    if(onDisconnectCallback != nullptr)
        onDisconnectCallback();
}
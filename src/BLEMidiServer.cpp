#include "BLEMidiServer.h"

BLEMidiServer::BLEMidiServer(
    const std::string deviceName,
    const void (*onConnectCallback)(BLEServer*),
    const void (*onDisconnectCallback)(BLEServer*)
    ) 
    :   BLEMidi(deviceName), 
        onConnectCallback(onConnectCallback), 
        onDisconnectCallback(onDisconnectCallback)
{}

void BLEMidiServer::begin()
{
    BLEMidi::begin();
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(&connected));
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
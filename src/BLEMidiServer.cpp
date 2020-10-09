#include "BLEMidiServer.h"

BLEMidiServer::BLEMidiServer(const std::string deviceName) : BLEMidi(deviceName) { }

void BLEMidiServer::begin()
{
    BLEMidi::begin();
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(&connected, onConnectCallback, onDisconnectCallback));
    BLEService *pService = pServer->createService(BLEUUID(MIDI_SERVICE_UUID));
    pCharacteristic = pService->createCharacteristic(
        BLEUUID(MIDI_CHARACTERISTIC_UUID),
        BLECharacteristic::PROPERTY_READ   |
        BLECharacteristic::PROPERTY_WRITE  |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_WRITE_NR
    );
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new CharacteristicCallback([this](uint8_t *data, uint8_t size) { this->receivePacket(data, size); }));
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();
}

void BLEMidiServer::setOnConnectCallback(void (*const onConnectCallback)())
{
    this->onConnectCallback = onConnectCallback;
}

void BLEMidiServer::setOnDisconnectCallback(void (*const onDisconnectCallback)())
{
    this->onDisconnectCallback = onDisconnectCallback;
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

CharacteristicCallback::CharacteristicCallback(std::function<void(uint8_t*, uint8_t)> onWriteCallback) : onWriteCallback(onWriteCallback) {}

void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0 && onWriteCallback != nullptr)
        onWriteCallback((uint8_t*)rxValue.c_str(), rxValue.length());
    
}
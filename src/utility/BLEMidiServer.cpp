#include "BLEMidiServer.h"

void BLEMidiServerClass::begin(const std::string deviceName)
{
    BLEMidi::begin(deviceName);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);
    BLEService *pService = pServer->createService(BLEUUID(MIDI_SERVICE_UUID));
    pCharacteristic = pService->createCharacteristic(
        BLEUUID(MIDI_CHARACTERISTIC_UUID),
        NIMBLE_PROPERTY::READ   |
        NIMBLE_PROPERTY::WRITE  |
        NIMBLE_PROPERTY::NOTIFY |
        NIMBLE_PROPERTY::WRITE_NR
    );
    pCharacteristic->setCallbacks(new CharacteristicCallback([this](uint8_t *data, uint8_t size) { this->receivePacket(data, size); }));
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    auto data = pAdvertising->getAdvertisementData();
    data.setName(deviceName);
    pAdvertising->setAdvertisementData(data);
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();
}

void BLEMidiServerClass::setOnConnectCallback(void (*const onConnectCallback)())
{
    this->onConnectCallback = onConnectCallback;
}

void BLEMidiServerClass::setOnDisconnectCallback(void (*const onDisconnectCallback)())
{
    this->onDisconnectCallback = onDisconnectCallback;
}

void BLEMidiServerClass::sendPacket(uint8_t *packet, uint8_t packetSize)
{
    if(!connected)
        return;
    pCharacteristic->setValue(packet, packetSize);
    pCharacteristic->notify();
}

void BLEMidiServerClass::onConnect(BLEServer* pServer, NimBLEConnInfo& connInfo)
{
    connected = true;
    if(onConnectCallback != nullptr)
        onConnectCallback();
}

void BLEMidiServerClass::onDisconnect(BLEServer* pServer, NimBLEConnInfo& connInfo, int reason)
{
    connected = false;
    if(onDisconnectCallback != nullptr)
        onDisconnectCallback();
    pServer->startAdvertising();
}

CharacteristicCallback::CharacteristicCallback(std::function<void(uint8_t*, uint8_t)> onWriteCallback) : onWriteCallback(onWriteCallback) {}

void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0 && onWriteCallback != nullptr)
        onWriteCallback((uint8_t*)rxValue.c_str(), rxValue.length());

    vTaskDelay(0);      // We leave some time for the IDLE task call esp_task_wdt_reset_watchdog
                        // See comment from atanisoft here : https://github.com/espressif/arduino-esp32/issues/2493

}

BLEMidiServerClass BLEMidiServer;
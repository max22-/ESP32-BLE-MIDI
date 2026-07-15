#include "BLEMidiClient.h"

void BLEMidiClientClass::begin(const std::string deviceName)
{
    BLEMidi::begin(deviceName);
}

unsigned int BLEMidiClientClass::scan()
{
    debug.println("Beginning scan...");
    pBLEScan = NimBLEDevice::getScan();
    if(pBLEScan == nullptr)
        return 0;
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->clearResults();
    scanResults = pBLEScan->getResults(3000, false);
    debug.printf("Found %d BLE device(s)\n", scanResults.getCount());
    return scanResults.getCount();
}

String BLEMidiClientClass::deviceName(unsigned int deviceIndex) {
    if(deviceIndex >= scanResults.getCount()) {
        debug.printf("warning: deviceName(): invalid index %u (%u devices found)\n", deviceIndex, scanResults.getCount());
        return "";
    }
    return String(scanResults.getDevice(deviceIndex)->getName().c_str());
}

String BLEMidiClientClass::deviceMacAddress(unsigned int deviceIndex) {
    if(deviceIndex >= scanResults.getCount()) {
        debug.printf("warning: deviceMacAddress(): invalid index %u (%u devices found)\n", deviceIndex, scanResults.getCount());
        return "";
    }
    return String(scanResults.getDevice(deviceIndex)->getAddress().toString().c_str());
}

bool BLEMidiClientClass::connect(unsigned int deviceIndex)
{
    debug.printf("Connecting to device number %d\n", deviceIndex);
    if(deviceIndex >= scanResults.getCount()) {
        debug.println("Cannot connect : device index is greater than the size of the MIDI devices list.");
        return false;
    }
    const NimBLEAdvertisedDevice* device = scanResults.getDevice(deviceIndex);
    if(device == nullptr)
        return false;
    debug.printf("Address of the device : %s\n", device->getAddress().toString().c_str());
    NimBLEClient* pClient = NimBLEDevice::createClient();
    debug.println("Registering callbacks");
    pClient->setClientCallbacks(new ClientCallbacks(connected, onConnectCallback, onDisconnectCallback));
    debug.println("pClient->connect()");
    if(!pClient->connect(device))
        return false;
    debug.println("pClient->getService()");
    NimBLERemoteService* pRemoteService = pClient->getService(MIDI_SERVICE_UUID.c_str());
    if(pRemoteService == nullptr) {
        debug.println("Couldn't find remote service");
        return false;
    }
    debug.println("pRemoteService->getCharacteristic()");
    pRemoteCharacteristic = pRemoteService->getCharacteristic(MIDI_CHARACTERISTIC_UUID.c_str());
    if(pRemoteCharacteristic == nullptr) {
        debug.println("Couldn't find remote characteristic");
        return false;
    }
    debug.println("Registering characteristic callback");
    if(pRemoteCharacteristic->canNotify()) {
        pRemoteCharacteristic->subscribe(true, [](NimBLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
            BLEMidiClient.receivePacket(pData, length); // We call the member function of the only instantiated class.
            vTaskDelay(0);      // We leave some time for the IDLE task call esp_task_wdt_reset_watchdog
                                // See comment from atanisoft here : https://github.com/espressif/arduino-esp32/issues/2493
        });
    }
    connected=true;
    return true;
}

void BLEMidiClientClass::setOnConnectCallback(void (*const onConnectCallback)())
{
    this->onConnectCallback = onConnectCallback;
}
void BLEMidiClientClass::setOnDisconnectCallback(void (*const onDisconnectCallback)())
{
    this->onDisconnectCallback = onDisconnectCallback;
}

void BLEMidiClientClass::sendPacket(uint8_t *packet, uint8_t packetSize)
{
    if(!connected)
        return;
    pRemoteCharacteristic->writeValue(packet, packetSize, false);
}

ClientCallbacks::ClientCallbacks(
    bool& connected,
    void (*onConnectCallback)(), 
    void (*onDisconnectCallback)()
) :     connected(connected),
        onConnectCallback(onConnectCallback),
        onDisconnectCallback(onDisconnectCallback)
{}

void ClientCallbacks::onConnect(NimBLEClient *pClient)
{
    connected = true;
    if(onConnectCallback != nullptr)
        onConnectCallback();
}

void ClientCallbacks::onDisconnect(NimBLEClient *pClient, int reason)
{
    connected = false;
    if(onDisconnectCallback != nullptr)
        onDisconnectCallback();
}

BLEMidiClientClass BLEMidiClient;
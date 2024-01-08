#include "BLEMidiClient.h"
#include "AdvertisedDeviceCallbacks.h"

void BLEMidiClientClass::begin(const std::string deviceName)
{
    BLEMidi::begin(deviceName);
}

/// @param vParams 
void BLEMidiClientClass::connect_task(void *vParams)
{
    BLEMidiClientClass *midi = (BLEMidiClientClass *)vParams;

    for (;;)
    {
        if (!midi->connected && midi->advertisedDeviceCallbacks && midi->advertisedDeviceCallbacks->getDevice())
        {
            midi->connect(midi->advertisedDeviceCallbacks->getDevice());
            delete midi->advertisedDeviceCallbacks;
            midi->advertisedDeviceCallbacks = nullptr;
            vTaskDelete(NULL);
        }
    }
}

int BLEMidiClientClass::scan()
{
    debug.println("Beginning scan...");
    pBLEScan = BLEDevice::getScan();
    if (pBLEScan == nullptr)
        return 0;
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->clearResults();
    foundMidiDevices.clear();
    BLEScanResults foundDevices = pBLEScan->start(3);
    debug.printf("Found %d BLE device(s)\n", foundDevices.getCount());
    for (int i = 0; i < foundDevices.getCount(); i++)
    {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        auto deviceStr = "name = \"" + device.getName() + "\", address = " + device.getAddress().toString();
        if (device.haveServiceUUID() && device.isAdvertisingService(BLEUUID(MIDI_SERVICE_UUID)))
        {
            debug.println((" - BLE MIDI device : " + deviceStr).c_str());
            foundMidiDevices.push_back(device);
        }
        else
            debug.println((" - Other type of BLE device : " + deviceStr).c_str());
        debug.printf("Total of BLE MIDI devices : %d\n", foundMidiDevices.size());
    }
    return foundMidiDevices.size();
}

BLEAdvertisedDevice *BLEMidiClientClass::backgroundScan(std::string const &name, std::string const &address)
{
    if (advertisedDeviceCallbacks)
    {
        delete (advertisedDeviceCallbacks);
        advertisedDeviceCallbacks = nullptr;
    }
    if (!advertisedDeviceCallbacks)
        advertisedDeviceCallbacks = new AdvertisedDeviceCallbacks(name, address, this);
    if (BLEDevice::getScan()->isScanning())
        return NULL;
    if (advertisedDeviceCallbacks->getDevice())
        return advertisedDeviceCallbacks->getDevice();

    xTaskCreate(connect_task, "connect", 2048, this, tskIDLE_PRIORITY, nullptr);

    BLEScan *pScan = BLEDevice::getScan();
    pScan->setAdvertisedDeviceCallbacks(advertisedDeviceCallbacks);
    pScan->setInterval(0x50);
    pScan->setWindow(0x30);
    pScan->setActiveScan(true);
    pScan->start(0, nullptr, false);
    return NULL;
}

BLEAdvertisedDevice *BLEMidiClientClass::getScannedDevice(uint32_t deviceIndex)
{
    if (deviceIndex >= foundMidiDevices.size())
    {
        debug.println("Scanned device not found because requested index is greater than the devices list");
        return nullptr;
    }
    return &foundMidiDevices.at(deviceIndex);
}

bool BLEMidiClientClass::connect(BLEAdvertisedDevice *device)
{
    if (device == nullptr)
        return false;

    BLEClient *pClient = NULL;

    if (BLEDevice::getClientListSize())
    {
        pClient = BLEDevice::getClientByPeerAddress(device->getAddress());
        if (pClient)
        {
            debug.println("Got client by address");
            if (!pClient->connect(device, false))
            {
                debug.println("Reconnect failed");
                return false;
            }
            debug.println("Reconnected client");
        }
        else
        {
            pClient = BLEDevice::getDisconnectedClient();
        }
    }

    if (!pClient)
    {
        if (BLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS)
        {
            debug.println("Max clients reached - no more connections available");
            return false;
        }

        pClient = BLEDevice::createClient();

        debug.println("New client created");

        pClient->setClientCallbacks(new ClientCallbacks(connected, onConnectCallback, onDisconnectCallback));

        if (!pClient->connect(device))
        {
            /** Created a client but failed to connect, don't need to keep it as it has no data */
            BLEDevice::deleteClient(pClient);
            debug.println("Failed to connect, deleted client");
            return false;
        }
    }

    if (!pClient->isConnected())
    {
        if (!pClient->connect(device))
        {
            debug.println("Failed to connect");
            return false;
        }
    }

    debug.print("Connected to: ");
    debug.println(pClient->getPeerAddress().toString().c_str());
    debug.print("RSSI: ");
    debug.println(pClient->getRssi());

    debug.println("pClient->getService()");
    BLERemoteService *pRemoteService = pClient->getService(MIDI_SERVICE_UUID.c_str());
    if (pRemoteService == nullptr)
    {
        debug.println("Couldn't find remote service");
        return false;
    }
    debug.println("pRemoteService->getCharacteristic()");
    pRemoteCharacteristic = pRemoteService->getCharacteristic(MIDI_CHARACTERISTIC_UUID.c_str());
    if (pRemoteCharacteristic == nullptr)
    {
        debug.println("Couldn't find remote characteristic");
        return false;
    }
    debug.println("Registering characteristic callback");
    if (pRemoteCharacteristic->canNotify())
    {
        pRemoteCharacteristic->subscribe(true, [](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
                                         {
                                             BLEMidiClient.receivePacket(pData, length); // We call the member function of the only instantiated class.
                                             vTaskDelay(0);                              // We leave some time for the IDLE task call esp_task_wdt_reset_watchdog
                                                                                         // See comment from atanisoft here : https://github.com/espressif/arduino-esp32/issues/2493
                                         });
    }
    connected = true;
    return true;
}

bool BLEMidiClientClass::connect(uint32_t deviceIndex)
{
    debug.printf("Connecting to device number %d\n", deviceIndex);
    if (deviceIndex >= foundMidiDevices.size())
    {
        debug.println("Cannot connect : device index is greater than the size of the MIDI devices lists.");
        return false;
    }
    BLEAdvertisedDevice *device = new BLEAdvertisedDevice(foundMidiDevices.at(deviceIndex));

    return connect(device);
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
    if (!connected)
        return;
    pRemoteCharacteristic->writeValue(packet, packetSize, false);
}

ClientCallbacks::ClientCallbacks(
    bool &connected,
    void (*onConnectCallback)(),
    void (*onDisconnectCallback)()) : connected(connected),
                                      onConnectCallback(onConnectCallback),
                                      onDisconnectCallback(onDisconnectCallback)
{
}

void ClientCallbacks::onConnect(BLEClient *pClient)
{
    connected = true;
    if (onConnectCallback != nullptr)
        onConnectCallback();
}

void ClientCallbacks::onDisconnect(BLEClient *pClient)
{
    connected = false;
    if (onDisconnectCallback != nullptr)
        onDisconnectCallback();
}

BLEMidiClientClass BLEMidiClient;

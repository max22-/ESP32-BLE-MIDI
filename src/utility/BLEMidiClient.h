#ifndef BLE_MIDI_CLIENT_H
#define BLE_MIDI_CLIENT_H

#include <vector>
#include "BLEMidiBase.h"

class AdvertisedDeviceCallbacks;

class BLEMidiClientClass : public BLEMidi {
public:

    /// Initializes the BLEMidiClient
    void begin(const std::string deviceName) override;

    /// Begins a scan, and returns the number of MIDI devices found.
    int scan();

    /// Begins a scan and returns immediatelly. Connects to any device that has both the name and address passed to the search.
    /// If either is empty, it counts as a wildcard.
    /// If both are empty, it connects to the first midi device found  
    BLEAdvertisedDevice* backgroundScan(std::string const& name = "", std::string const& address = "");

    /// Returns the nth scanned MIDI device, or nullptr in case of an error. 
    /// Do not use the returned value if you perform another scan later, because it will be cleared.
    BLEAdvertisedDevice* getScannedDevice(uint32_t deviceIndex);

    /// Connect to the specified MIDI device
    bool connect(BLEAdvertisedDevice* pDevice);

    /// Connects to the nth scanned MIDI device
    bool connect(uint32_t deviceIndex = 0);

    void setOnConnectCallback(void (*const onConnectCallback)());
    void setOnDisconnectCallback(void (*const onDisconnectCallback)());


private:
    /// Task that connects a found midi device in the active thread.
    static void connect_task(void* vParams);
    /// This method is called by the base Midi class to send packets.
    virtual void sendPacket(uint8_t *packet, uint8_t packetSize) override;

    BLEScan *pBLEScan = nullptr;
    std::vector<BLEAdvertisedDevice> foundMidiDevices;
    BLERemoteCharacteristic* pRemoteCharacteristic;
    void (*onConnectCallback)() = nullptr;
    void (*onDisconnectCallback)() = nullptr;
    
    AdvertisedDeviceCallbacks *advertisedDeviceCallbacks = nullptr;

    friend class AdvertisedDeviceCallbacks;

};

/// Callbacks for connections and disconnections
class ClientCallbacks : public BLEClientCallbacks {
public:
    ClientCallbacks(
            bool& connected,
            void (* const onConnectCallback)() = nullptr, 
            void (* const onDisconnectCallback)() = nullptr
    );
private:
    void onConnect(BLEClient*);
    void onDisconnect(BLEClient*);

    bool& connected;
    void (* const onConnectCallback)();
    void (* const onDisconnectCallback)();
};


extern BLEMidiClientClass BLEMidiClient;

#endif
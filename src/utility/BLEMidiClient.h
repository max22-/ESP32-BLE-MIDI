#ifndef BLE_MIDI_CLIENT_H
#define BLE_MIDI_CLIENT_H

#include <vector>
#include "BLEMidiBase.h"

class BLEMidiClientClass : public BLEMidi {
public:

    /// Initializes the BLEMidiClient
    void begin(const std::string deviceName) override;

    /// Begins a scan, and returns the number of MIDI devices found.
    unsigned int scan();

    /// @brief Get a device name by its index.
    /// @param deviceIndex 0 to n-1, n being the number of devices returned by scan()
    /// @return The name of the device, or an empty string if the index is invalid.
    String deviceName(unsigned int deviceIndex);

    /// @brief Get a device mac address by its index.
    /// @param deviceIndex 0 to n-1, n being the number of devices returned by scan()
    /// @return The mac address of the device, or an empty string if the index is invalid.
    String deviceMacAddress(unsigned int deviceIndex);

    /// Connects to the nth scanned MIDI device
    bool connect(unsigned int deviceIndex);

    void setOnConnectCallback(void (*const onConnectCallback)());
    void setOnDisconnectCallback(void (*const onDisconnectCallback)());


private:
    /// This method is called by the base Midi class to send packets.
    virtual void sendPacket(uint8_t *packet, uint8_t packetSize) override;

    NimBLEScan *pBLEScan = nullptr;
    NimBLEScanResults scanResults;
    NimBLERemoteCharacteristic* pRemoteCharacteristic;
    void (*onConnectCallback)() = nullptr;
    void (*onDisconnectCallback)() = nullptr;

};

/// Callbacks for connections and disconnections
class ClientCallbacks : public NimBLEClientCallbacks {
public:
    ClientCallbacks(
            bool& connected,
            void (* const onConnectCallback)() = nullptr, 
            void (* const onDisconnectCallback)() = nullptr
    );
private:
    void onConnect(NimBLEClient *pClient) override;
    void onDisconnect(NimBLEClient *pClient, int reason) override;

    bool& connected;
    void (* const onConnectCallback)();
    void (* const onDisconnectCallback)();
};

extern BLEMidiClientClass BLEMidiClient;

#endif
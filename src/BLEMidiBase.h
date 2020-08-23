#ifndef BLE_MIDI_BASE_H
#define BLE_MIDI_BASE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

class MidiCallbacks {
public:
  void (*noteOn)(uint8_t channel, uint8_t note, uint8_t velocity) = nullptr;
  void (*noteOff)(uint8_t channel, uint8_t note, uint8_t velocity) = nullptr;
};


class BLEMidi {
public:
    BLEMidi(const std::string deviceName);
    virtual void begin();
    bool isConnected();
    void sendPacket(uint8_t *packet, uint8_t packetSize);

    void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void controlChange(uint8_t channel, uint8_t controller, uint8_t value);
    void programChange(uint8_t channel, uint8_t program);
    void pitchBend(uint8_t channel, uint8_t msb, uint8_t lsb);
    void pitchBend(uint8_t channel, float semitones);   // semitones : from -2 to + 2 semitones.
                                                        // Numbers outside of this range will be ignored.

    void setNoteOnCallback(void (*callback)(uint8_t channel, uint8_t note, uint8_t velocity));
    void setNoteOffCallback(void (*callback)(uint8_t channel, uint8_t note, uint8_t velocity));

protected:
    const std::string deviceName;
    BLECharacteristic *pCharacteristic;
    bool connected = false;
    const std::string SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
    const std::string CHARACTERISTIC_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3";
    MidiCallbacks midiCallbacks;

};


class CharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic);
    MidiCallbacks& midiCallbacks;
public:
    CharacteristicCallback(MidiCallbacks& callbacks);
};

#endif
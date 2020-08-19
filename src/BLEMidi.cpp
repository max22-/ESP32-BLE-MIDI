#include <Arduino.h>
#include <BLEMidi.h>

BLEMidi::BLEMidi(std::string deviceName) : deviceName(deviceName)
{

}

void BLEMidi::begin()
{
    BLEDevice::init(deviceName);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(&deviceConnected));
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

bool BLEMidi::isConnected()
{
    return deviceConnected;
}

void BLEMidi::sendPacket(uint8_t *packet, uint8_t packetSize)
{
    if(!deviceConnected)
        return;
    pCharacteristic->setValue(packet, packetSize);
    pCharacteristic->notify();
}

void BLEMidi::noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    uint8_t midiPacket[] = {
        0x80,  // header
        0x80,  // timestamp, not implemented 
        0x90 | channel, // 0x90 : note on
        note,
        velocity
    };
    if(channel > 15)
        return;
    if(note > 127)
        return;
    if(velocity > 127)
        return;
    sendPacket(midiPacket, sizeof(midiPacket));
}

void BLEMidi::noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
    uint8_t midiPacket[] = {
        0x80,  // header
        0x80,  // timestamp, not implemented 
        0x80 | channel, // 0x80 : note off
        note,
        velocity
    };
    if(channel > 15)
        return;
    if(note > 127)
        return;
    if(velocity > 127)
        return;
    sendPacket(midiPacket, sizeof(midiPacket));
}

void BLEMidi::controlChange(uint8_t channel, uint8_t controller, uint8_t value)
{
    uint8_t midiPacket[] = {
        0x80,  // header
        0x80,  // timestamp, not implemented 
        0xB0 | channel, // 0xB0 : control change
        controller,
        value
    };
    if(channel > 15)
        return;
    if(controller > 127)
        return;
    if(value > 127)
        return;
    sendPacket(midiPacket, sizeof(midiPacket));
}
void BLEMidi::programChange(uint8_t channel, uint8_t program)
{
    uint8_t midiPacket[] = {
        0x80,  // header
        0x80,  // timestamp, not implemented 
        0xC0 | channel, // 0xA0 : program change
        program,
    };
    if(channel > 15)
        return;
    if(program > 127)
        return;
    sendPacket(midiPacket, sizeof(midiPacket));
}

void BLEMidi::setNoteOnCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    midiCallbacks.noteOn = callback;
}

void BLEMidi::setNoteOffCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    midiCallbacks.noteOff = callback;
}

CharacteristicCallback::CharacteristicCallback(MidiCallbacks& midiCallbacks) : midiCallbacks(midiCallbacks) {}

void CharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
        switch(rxValue[2] >> 4) {
            case 0x9:
                if(midiCallbacks.noteOn != nullptr && rxValue.length() >= 5)
                    midiCallbacks.noteOn(rxValue[2] & 0xF, rxValue[3], rxValue[4]);
                break;
            case 0x8:
                if(midiCallbacks.noteOff != nullptr && rxValue.length() >= 5)
                    midiCallbacks.noteOff(rxValue[2] & 0xF, rxValue[3], rxValue[4]);
                break;
            
        }
    }
}
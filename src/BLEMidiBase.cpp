#include <Arduino.h>
#include <BLEMidiBase.h>

BLEMidi::BLEMidi(const std::string deviceName) : deviceName(deviceName)
{

}

void BLEMidi::begin()
{
    BLEDevice::init(deviceName);
}

bool BLEMidi::isConnected()
{
    return connected;
}

void BLEMidi::sendPacket(uint8_t *packet, uint8_t packetSize)
{
    if(!connected)
        return;
    pCharacteristic->setValue(packet, packetSize);
    pCharacteristic->notify();
}

void BLEMidi::noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    uint8_t midiPacket[] = {
        0x80,  // header
        0x80,  // timestamp, not implemented 
        (uint8_t)(0x90 | channel), // 0x90 : note on
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
        (uint8_t)(0x80 | channel), // 0x80 : note off
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
        (uint8_t)(0xB0 | channel), // 0xB0 : control change
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
        (uint8_t)(0xC0 | channel), // 0xC0 : program change
        program,
    };
    if(channel > 15)
        return;
    if(program > 127)
        return;
    sendPacket(midiPacket, sizeof(midiPacket));
}

void BLEMidi::pitchBend(uint8_t channel, uint8_t lsb, uint8_t msb) {
    uint8_t midiPacket[] = {
        0x80,  // header
        0x80,  // timestamp, not implemented 
        (uint8_t)(0xE0 | channel), // 0xE0 : pitch bend
        lsb,
        msb
    };
    sendPacket(midiPacket, sizeof(midiPacket));
}

void BLEMidi::pitchBend(uint8_t channel, float semitones)
{
    if(semitones < -2 || semitones > 2)
        return;
    uint16_t tmp = (semitones + 2) * ((1<<14)-1) / 4;
    tmp = tmp << 1;
    byte msb = highByte(tmp);
    byte lsb = lowByte(tmp) >> 1;
    pitchBend(channel, lsb, msb);
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
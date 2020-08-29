#include "Midi.h"

void Midi::noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
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

void Midi::noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
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

void Midi::controlChange(uint8_t channel, uint8_t controller, uint8_t value)
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
void Midi::programChange(uint8_t channel, uint8_t program)
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

void Midi::pitchBend(uint8_t channel, uint8_t lsb, uint8_t msb) {
    uint8_t midiPacket[] = {
        0x80,  // header
        0x80,  // timestamp, not implemented 
        (uint8_t)(0xE0 | channel), // 0xE0 : pitch bend
        lsb,
        msb
    };
    sendPacket(midiPacket, sizeof(midiPacket));
}

void Midi::pitchBend(uint8_t channel, float semitones)
{
    if(semitones < -2 || semitones > 2)
        return;
    uint16_t tmp = (semitones + 2) * ((1<<14)-1) / 4;
    tmp = tmp << 1;
    byte msb = highByte(tmp);
    byte lsb = lowByte(tmp) >> 1;
    pitchBend(channel, lsb, msb);
}

// ###################################
// IO

void Midi::receivePacket(uint8_t *data, uint8_t size)
{
    Serial.print("Received data : ");
    for(uint8_t i=0; i<size; i++)
        Serial.printf("%x ", data[i]);
    Serial.println();
}

// ###################################
// Callbacks

void Midi::setNoteOnCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    noteOnCallback = callback;
}

void Midi::setNoteOffCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    noteOffCallback = callback;
}

void Midi::setControlChangeCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    controlChangeCallback = callback;
}

void Midi::setProgramChangeCallback(void (*callback)(uint8_t, uint8_t))
{
    programChangeCallback = callback;
}


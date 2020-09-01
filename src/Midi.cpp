#include "Midi.h"

/*
Midi documentation found here :
https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message

Pitch bend:
https://sites.uci.edu/camp2014/2014/04/30/managing-midi-pitchbend-messages/
*/

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
    //check data !!!!
    uint8_t status = data[2];
    uint8_t message = status >> 4;
    uint8_t channel = status & 0xF;

    uint8_t note = data[3];
    uint8_t velocity = data[4];
    uint8_t polyPressure = data[4];
    uint8_t controller = data[3];
    uint8_t controllerValue = data[4];
    uint8_t program = data[3];
    uint8_t channelPressure = data[3];
    uint8_t lsb = data[3], msb = data[4];

    switch(message) {

        case 0b1000:    // Note off
            if(noteOffCallback != nullptr)
                noteOffCallback(channel, note, velocity);
            Serial.printf("Note off, channel %d, note %d, velocity %d\n", channel, note, velocity);
            break;

        case 0b1001:    // Note on
            if(noteOnCallback != nullptr)
                noteOnCallback(channel, note, velocity);
            Serial.printf("Note on, channel %d, note %d, velocity %d\n", channel, note, velocity);
            break;

        case 0b1010:    // Polyphonic key pressure (Aftertouch)
            Serial.printf("Polyphonic key pressure, channel %d, note %d, velocity %d\n", channel, note, polyPressure);
            break;

        case 0b1011:    // Control Change
            if(controlChangeCallback != nullptr)
                controlChangeCallback(channel, controller, controllerValue);
            Serial.printf("Control Change, channel %d, controller %d, value %d\n", channel, controller, controllerValue);
            if(controller >= 120) { // Reserved controllers, for "Channel Mode Messages"
                Serial.println("Reserved controller, not implemented yet");
                switch(controller) {
                    case 120:       // All sounds off
                        break;
                    case 121:       // Reset all controllers
                        break;
                    case 122:       // Local control
                        break;
                    case 123:       // All Notes off
                        break;
                    case 124:       // Omni Mode Off
                        break;
                    case 125:       // Omni Mode On
                        break;
                    case 126:       // Mono Mode on (Poly off)
                        break;
                    case 127:       // Poly Mode on (Mono off)
                        break;  
                    default:
                        break;
                }
            }
            break;

        case 0b1100:    // Program Change
            if(programChangeCallback != nullptr)
                programChangeCallback(channel, program);
            Serial.printf("Program Change, channel %d, program %d\n", channel, program);
            break;

        case 0b1101:    // Channel pressure (Aftertouch)
            Serial.printf("Channel pressure, channel %d, pressure %d\n", channel, channelPressure);
            break;

        case 0b1110:    // Pitch bend
            {   // block because we declare variables (otherwise compile error)
            Serial.printf("Pitch bend, channel %d, lsb %d, msb %d\n", channel, lsb, msb);
            uint16_t integerPitchBend = ((msb & 127) << 7) | (lsb & 127);
            Serial.printf("Integer value of pitch bend : %d\n", integerPitchBend);
            float semitones = 4*(float)(integerPitchBend - 8192)/powf(2, 14);
            Serial.printf("Pitch bend in semitones : %.2f\n", semitones);
            break;
            }

        case 0b1111:
            Serial.println("System common message, not implemented yet");
            break;

        default:
            break;
        
    }

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

void Midi::enableDebugging(Stream& debugStream) {
    debug.enable(debugStream);
}

void Midi::disableDebugging()
{
    debug.disable();
}
#include "Midi.h"

/*
Midi documentation found here :
https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message

Pitch bend:
https://sites.uci.edu/camp2014/2014/04/30/managing-midi-pitchbend-messages/
*/

void Midi::noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    uint8_t midiMessage[] = {
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
    sendMessage(midiMessage, sizeof(midiMessage));
}

void Midi::noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
    uint8_t midiMessage[] = {
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
    sendMessage(midiMessage, sizeof(midiMessage));
}

void Midi::controlChange(uint8_t channel, uint8_t controller, uint8_t value)
{
    uint8_t midiMessage[] = {
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
    sendMessage(midiMessage, sizeof(midiMessage));
}
void Midi::programChange(uint8_t channel, uint8_t program)
{
    uint8_t midiMessage[] = {
        (uint8_t)(0xC0 | channel), // 0xC0 : program change
        program,
    };
    if(channel > 15)
        return;
    if(program > 127)
        return;
    sendMessage(midiMessage, sizeof(midiMessage));
}

void Midi::pitchBend(uint8_t channel, uint8_t lsb, uint8_t msb) {
    uint8_t midiMessage[] = {
        (uint8_t)(0xE0 | channel), // 0xE0 : pitch bend
        lsb,
        msb
    };
    sendMessage(midiMessage, sizeof(midiMessage));
}

void Midi::pitchBend(uint8_t channel, float semitones, float range)
{
    if(semitones < -range/2 || semitones > range/2)
        return;
    uint16_t tmp = semitones * 16384 / range + 8192;
    tmp = tmp << 1;
    byte msb = highByte(tmp);
    byte lsb = lowByte(tmp) >> 1;
    pitchBend(channel, lsb, msb);
}

// ###################################
// IO

void Midi::receivePacket(uint8_t *data, uint8_t size)
{
    debug.print("Received data : ");
    for(uint8_t i=0; i<size; i++)
        debug.printf("%x ", data[i]);
    debug.println();
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
            debug.printf("Note off, channel %d, note %d, velocity %d\n", channel, note, velocity);
            break;

        case 0b1001:    // Note on
            if(noteOnCallback != nullptr)
                noteOnCallback(channel, note, velocity);
            debug.printf("Note on, channel %d, note %d, velocity %d\n", channel, note, velocity);
            break;

        case 0b1010:    // Polyphonic key pressure (Aftertouch)
            debug.printf("Polyphonic key pressure, channel %d, note %d, velocity %d\n", channel, note, polyPressure);
            break;

        case 0b1011:    // Control Change
            if(controlChangeCallback != nullptr)
                controlChangeCallback(channel, controller, controllerValue);
            debug.printf("Control Change, channel %d, controller %d, value %d\n", channel, controller, controllerValue);
            if(controller >= 120) { // Reserved controllers, for "Channel Mode Messages"
                debug.println("Reserved controller, not implemented yet");
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
            debug.printf("Program Change, channel %d, program %d\n", channel, program);
            break;

        case 0b1101:    // Channel pressure (Aftertouch)
            debug.printf("Channel pressure, channel %d, pressure %d\n", channel, channelPressure);
            break;

        case 0b1110:    // Pitch bend
            {   // block because we declare variables (otherwise compile error)
            if(pitchBendCallback != nullptr)
                pitchBendCallback(channel, lsb, msb);
            debug.printf("Pitch bend, channel %d, lsb %d, msb %d\n", channel, lsb, msb);
            uint16_t integerPitchBend = ((msb & 127) << 7) | (lsb & 127);
            if(pitchBendCallback2 != nullptr)
                pitchBendCallback2(channel, integerPitchBend);
            debug.printf("Integer value of pitch bend : %d\n", integerPitchBend);
            float semitones = 4*(float)(integerPitchBend - 8192)/powf(2, 14);
            debug.printf("Pitch bend in semitones : %.2f\n", semitones);
            break;
            }

        case 0b1111:
            debug.println("System common message, not implemented yet");
            break;

        default:
            break;
        
    }

}

void Midi::sendMessage(uint8_t *message, uint8_t messageSize)
{
    uint8_t packet[messageSize+2];

    auto t = millis();
    uint8_t headerByte = (1 << 7) | ((t >> 7) & ((1 << 6) - 1));
    uint8_t timestampByte = (1 << 7) | (t & ((1 << 7) - 1));

    packet[0] = headerByte;
    packet[1] = timestampByte;
    for(int i = 0; i < messageSize; i++)
        packet[i+2] = message[i];
    sendPacket(packet, messageSize + 2);

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

void Midi::setPitchBendCallback(void (*callback)(uint8_t, uint8_t, uint8_t))
{
    pitchBendCallback = callback;
}

void Midi::setPitchBendCallback(void (*callback)(uint8_t, uint16_t))
{
    pitchBendCallback2 = callback;
}

void Midi::enableDebugging(Stream& debugStream) {
    debug.enable(debugStream);
}

void Midi::disableDebugging()
{
    debug.disable();
}
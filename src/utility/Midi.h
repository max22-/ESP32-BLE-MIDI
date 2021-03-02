#ifndef MIDI_H
#define MIDI_H

#include <Arduino.h>
#include "Debug.h"


class Midi {
public:
    void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void controlChange(uint8_t channel, uint8_t controller, uint8_t value);
    void programChange(uint8_t channel, uint8_t program);
    void pitchBend(uint8_t channel, uint8_t msb, uint8_t lsb);
    void pitchBend(uint8_t channel, float semitones);   /// semitones : from -2 to + 2 semitones.
                                                        /// Numbers outside of this range will be ignored.

    void setNoteOnCallback(void (*callback)(uint8_t channel, uint8_t note, uint8_t velocity));
    void setNoteOffCallback(void (*callback)(uint8_t channel, uint8_t note, uint8_t velocity));
    void setControlChangeCallback(void (*callback)(uint8_t channel, uint8_t controller, uint8_t value));
    void setProgramChangeCallback(void (*callback)(uint8_t channel, uint8_t program));
    void setPitchBendCallback(void (*callback)(uint8_t channel, uint8_t lsb, uint8_t msb));
    void setPitchBendCallback(void (*callback)(uint8_t channel, uint16_t value));

    void enableDebugging(Stream& debugStream = Serial);
    void disableDebugging();

protected:
    virtual void sendPacket(uint8_t *packet, uint8_t packetSize) = 0;
    void receivePacket(uint8_t *packet, uint8_t packetSize);
    Debug debug;

private:
    void sendMessage(uint8_t *message, uint8_t messageSize);
    void (*noteOnCallback)(uint8_t, uint8_t, uint8_t) = nullptr;
    void (*noteOffCallback)(uint8_t, uint8_t, uint8_t) = nullptr;
    void (*controlChangeCallback)(uint8_t, uint8_t, uint8_t) = nullptr;
    void (*programChangeCallback)(uint8_t, uint8_t) = nullptr;
    void (*pitchBendCallback)(uint8_t, uint8_t, uint8_t) = nullptr;
    void (*pitchBendCallback2)(uint8_t, uint16_t) = nullptr;
};

#endif
#ifndef MIDI_H
#define MIDI_H

#include <Arduino.h>
#include "Debug.h"


class Midi {
public:
    void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void afterTouchPoly(uint8_t channel, uint8_t note, uint8_t pressure);
    void controlChange(uint8_t channel, uint8_t controller, uint8_t value);
    void programChange(uint8_t channel, uint8_t program);
    void afterTouch(uint8_t channel, uint8_t pressure);
    void pitchBend(uint8_t channel, uint8_t lsb, uint8_t msb);
    /**
     * @param channel Midi channel
     * @param value Pitch bend value, from 0 to 16383
     * */
    void pitchBend(uint8_t channel, uint16_t value);
    /**
     * This function is useful if you do not want to calculate lsb and msb of the pitchbend value yourself.
     * @param channel Midi channel
     * @param semitones Number of semitones, from -range to +range (pitchbend values outside of range will be ignored)
     * @param range Range of the pitch bend in semitones (default value is 4, which is -2 to +2 semitones)
     * */
    void pitchBend(uint8_t channel, float semitones, float range = 4);

    void setNoteOnCallback(void (*callback)(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp));
    void setNoteOffCallback(void (*callback)(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp));
    void setAfterTouchPolyCallback(void (*callback)(uint8_t channel, uint8_t note, uint8_t pressure, uint16_t timestamp));
    void setControlChangeCallback(void (*callback)(uint8_t channel, uint8_t controller, uint8_t value, uint16_t timestamp));
    void setProgramChangeCallback(void (*callback)(uint8_t channel, uint8_t program, uint16_t timestamp));
    void setAfterTouchCallback(void (*callback)(uint8_t channel, uint8_t pressure, uint16_t timestamp));
    void setPitchBendCallback(void (*callback)(uint8_t channel, uint8_t lsb, uint8_t msb, uint16_t timestamp));
    void setPitchBendCallback(void (*callback)(uint8_t channel, uint16_t value, uint16_t timestamp));

    void enableDebugging(Stream& debugStream = Serial);
    void disableDebugging();

protected:
    virtual void sendPacket(uint8_t *packet, uint8_t packetSize) = 0;
    void receivePacket(uint8_t *packet, uint8_t packetSize);
    Debug debug;

private:
    void sendMessage(uint8_t *message, uint8_t messageSize);
    void (*noteOnCallback)(uint8_t, uint8_t, uint8_t, uint16_t) = nullptr;
    void (*noteOffCallback)(uint8_t, uint8_t, uint8_t, uint16_t) = nullptr;
    void (*afterTouchPolyCallback)(uint8_t, uint8_t, uint8_t, uint16_t) = nullptr;
    void (*controlChangeCallback)(uint8_t, uint8_t, uint8_t, uint16_t) = nullptr;
    void (*programChangeCallback)(uint8_t, uint8_t, uint16_t) = nullptr;
    void (*afterTouchCallback)(uint8_t, uint8_t, uint16_t) = nullptr;
    void (*pitchBendCallback)(uint8_t, uint8_t, uint8_t, uint16_t) = nullptr;
    void (*pitchBendCallback2)(uint8_t, uint16_t, uint16_t) = nullptr;

    uint16_t currentTimestamp = 0;
    
};

#endif
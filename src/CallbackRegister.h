#ifndef CALLBACK_REGISTER_H
#define CALLBACK_REGISTER_H

#include <unordered_map>
#include <BLERemoteCharacteristic.h>
#include <functional>

/*
This module is a wrapper to be able to use a member function of BLEMidiClient as a callback for
BLERemoteCharacteristic::registerForNotify(notify_callback notifyCallback, bool notifications)
which accepts only C-style callback functions.
I admit this is a bit overkill :)
*/

class BLEMidiClient;

namespace CallbackRegister {

    typedef std::function<void(BLERemoteCharacteristic*, uint8_t*, size_t, bool)> callback_t;

    void mainCallback(
        BLERemoteCharacteristic* pBLERemoteCharacteristic,
        uint8_t* pData,
        size_t length,
        bool isNotify
    );

    // We identify callbacks using the remote characteristic pointer.      
    void registerCallback(
        BLERemoteCharacteristic* pBLERemoteCharacteristic,  
        callback_t callback
    );

    /* 
    Callbacks are not removed from the register when the device gets disconnected.
    It would be cleaner to do it, but it consumes very little memory so... maybe later :)
    */

};

#endif
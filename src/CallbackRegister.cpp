#include "CallbackRegister.h"
#include "BLEMidiClient.h"


namespace CallbackRegister {

std::unordered_map<BLERemoteCharacteristic*, callback_t> _register;

void mainCallback(
        BLERemoteCharacteristic* pBLERemoteCharacteristic,
        uint8_t* pData,
        size_t length,
        bool isNotify) 
{
    _register[pBLERemoteCharacteristic](pBLERemoteCharacteristic, pData, length, isNotify);
}

void registerCallback(
    BLERemoteCharacteristic* pBLERemoteCharacteristic,  
    callback_t callback)
{
    _register[pBLERemoteCharacteristic] = callback;
}

}
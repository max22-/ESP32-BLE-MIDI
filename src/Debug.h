#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

class Debug : public Stream {
public:
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();
    virtual size_t write(uint8_t val);
    
    void enable(Stream& stream);
    void disable();

private:
    Stream *stream = nullptr;
};

#endif
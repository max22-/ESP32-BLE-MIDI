#include "Debug.h"

int Debug::available() {
    return 0;
}

int Debug::read()
{
    return -1;
}

int Debug::peek()
{
    return -1;
}

void Debug::flush()
{
    if(stream != nullptr)
        stream->flush();
}

size_t Debug::write(uint8_t val)
{
    if(stream != nullptr)
        return stream->write(val);
    return 0;
}

void Debug::enable(Stream& stream)
{
    this->stream = &stream;
}

void Debug::disable()
{
    stream = nullptr;
}


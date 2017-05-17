#ifndef ARDUINO_HPP
#define ARDUINO_HPP
#include "serial.hpp"
#include "../arduino/message.hpp"
#include <stdint.h>

/* const uint16_t MSGTYPE_READY = 0;
const uint16_t MSGTYPE_SET_BPM = 2;

struct Message
{
    uint16_t type, arg1, arg2, arg3;

    Message() {};
    Message(uint16_t t, uint16_t a1, uint16_t a2, uint16_t a3): type(t), arg1(a1), arg2(a2), arg3(a3) {};
};*/

class Arduino
{
public:
    Arduino();
    ~Arduino();
    bool isMessageAvailable();
    Message readMessage();
    void writeMessage(Message msg);

private:
    Serial serial;
};
#endif

#include "arduino.hpp"
#include <iostream>

Arduino::Arduino(): serial("/dev/ttyACM0")
{
}

Arduino::~Arduino()
{
}

bool Arduino::isMessageAvailable()
{
    return serial.getAvailable() >= 8;
}

Message Arduino::readMessage()
{
    char buf[8];
    serial.readBytes(buf, 8);

    Message msg;
    msg.type = (uint16_t)buf[0] * 256 + (uint16_t)buf[1];
    msg.arg1 = (uint16_t)buf[2] * 256 + (uint16_t)buf[3];
    msg.arg2 = (uint16_t)buf[4] * 256 + (uint16_t)buf[5];
    msg.arg3 = (uint16_t)buf[6] * 256 + (uint16_t)buf[7];

    return msg;
}

void Arduino::writeMessage(Message msg)
{
    char buf[8];

    buf[0] = msg.type / 256;
    buf[1] = msg.type % 256;

    buf[2] = msg.arg1 / 256;
    buf[3] = msg.arg1 % 256;

    buf[4] = msg.arg2 / 256;
    buf[5] = msg.arg2 % 256;

    buf[6] = msg.arg3 / 256;
    buf[7] = msg.arg3 % 256;

    serial.writeBytes(buf, 8);
}

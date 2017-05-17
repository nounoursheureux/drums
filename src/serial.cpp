#include "serial.hpp"
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

Serial::Serial(std::string path)
{
    fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        throw std::runtime_error("Could not open serial file");
    }
    fcntl(fd, F_SETFL, 0);
}

Serial::~Serial()
{
    close(fd);
}

int Serial::getAvailable()
{
    int bytes;
    if (ioctl(fd, FIONREAD, &bytes) == -1) {
        throw std::runtime_error("Could not get the available input size");
    }
    return bytes;
}

void Serial::readBytes(char* buf, unsigned int length)
{
    if (read(fd, buf, length) == -1) {
        throw std::runtime_error("Could not read from the serial file");
    }
}

void Serial::writeBytes(char* buf, unsigned int length)
{
    if (write(fd, buf, length) == -1) {
        throw std::runtime_error("Could not write to the serial file");
    }
}

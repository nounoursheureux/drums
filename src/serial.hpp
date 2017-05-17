#ifndef SERIAL_HPP
#define SERIAL_HPP
#include <string>

class Serial
{
public:
    explicit Serial(std::string path);
    ~Serial();
    int getAvailable();
    void readBytes(char* buf, unsigned int length);
    void writeBytes(char* buf, unsigned int length);

private:
    int fd;
};
#endif

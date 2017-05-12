#ifndef SAMPLE_HPP
#define SAMPLE_HPP
#include <cstdint>
#include <string>

struct SampleFile
{
    float* samples;
    uint64_t sampleCount;
    unsigned int channels;
    unsigned int sampleRate;

    explicit SampleFile(std::string path);
    SampleFile(float* pSamples, uint64_t pSampleCount, unsigned int pChannels, unsigned int pSampleRate);
};

class SampleBank
{

};
#endif

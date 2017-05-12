#ifndef SAMPLE_HPP
#define SAMPLE_HPP
#include <cstdint>
#include <string>
#include <map>

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
public:
    SampleBank();
    ~SampleBank();
    SampleFile* get(unsigned int id);

private:
    std::map<unsigned int, SampleFile*> samples;
};
#endif

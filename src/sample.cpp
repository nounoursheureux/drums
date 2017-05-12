#include "sample.hpp"
#include "dr_wav.h"
#include <stdexcept>

SampleFile::SampleFile(float* pSamples, uint64_t pSampleCount, unsigned int pChannels, unsigned int pSampleRate)
{
    samples = pSamples;
    sampleCount = pSampleCount;
    channels = pChannels;
    sampleRate = pSampleRate;
}

SampleFile::SampleFile(std::string path)
{
    samples = drwav_open_and_read_file_f32(path.c_str(), &channels, &sampleRate, &sampleCount);
    if (samples == nullptr) throw std::runtime_error("File not found");
}

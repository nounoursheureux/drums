#include "sample.hpp"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#include <stdexcept>
#include <cstdio>
#include <iostream>

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

SampleBank::SampleBank()
{
    for (unsigned int i = 0; i < 100; i++) {
        std::string path = std::string("samples/") + std::to_string(i);
        FILE* fp = fopen(path.c_str(), "rb");
        if (fp == nullptr) {
            continue;
        }

        std::cout << path << std::endl;
        fclose(fp);
        samples[i] = new SampleFile(path);
    }
}

SampleBank::~SampleBank()
{
    for (unsigned int i = 0; i < 100; i++) {
        auto it = samples.find(i);
        if (it != samples.end()) {
            delete it->second;
        }
    }
}

SampleFile* SampleBank::get(unsigned int id)
{
    return samples.at(id);
}

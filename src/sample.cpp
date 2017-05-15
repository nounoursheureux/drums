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
}

SampleBank::~SampleBank()
{
    for (auto it : samples) {
        delete it.second;
    }
}

void SampleBank::load(std::string path)
{
    // just check if the file exists
    FILE* fp = fopen(path.c_str(), "rb");
    if (fp == nullptr) {
        throw std::runtime_error("Sample not found");
    }

    std::cout << path << std::endl;
    fclose(fp);
    samples[path] = new SampleFile(path);
}

SampleFile* SampleBank::get(std::string path)
{
    auto it = samples.find(path);
    if (it == samples.end()) {
        load(path);
        return samples.at(path);
    }
    return it->second;
}

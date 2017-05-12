#ifndef SOUND_HPP
#define SOUND_HPP
#include "sample.hpp"

struct Sound
{
    SampleFile* sample;
    uint64_t offset;

    explicit Sound(SampleFile* pSample);
};
#endif

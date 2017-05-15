#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <vector>
#include "portaudio.h"
#include "sample.hpp"
#include "sequencer.hpp"
#include "sound.hpp"

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();
    Sequencer& getSequencer();
    SampleBank& getBank();
    void addSound(Sound* sound);
    void addSound(std::string path);
    void update();

private:
    void writeToStream(signed long num);

    PaStream* stream;
    std::vector<Sound*> sounds;
    Sequencer sequencer;
    SampleBank bank;
};
#endif

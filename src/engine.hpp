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
    void addSound(unsigned int sampleID);
    void setBpm(double bpm);
    void update();

private:
    void writeToStream(signed long num);

    PaStream* stream;
    std::vector<Sound*> sounds;
    Sequencer sequencer;
    SampleBank bank;
    PaTime latest_time, elapsed_time;
    double time_per_quarter_beat;
    unsigned int beat_number;
};
#endif

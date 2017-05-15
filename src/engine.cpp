#include "engine.hpp"
#include <stdexcept>
#include <iostream>

void handle_error(PaError err)
{
    if (err != paNoError) {
        throw std::runtime_error(Pa_GetErrorText(err));
        // fputs(Pa_GetErrorText(err), stderr);
        // exit(1);
    }
}

AudioEngine::AudioEngine(): sequencer(140.0)
{
    PaError err = Pa_Initialize();
    handle_error(err);

    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 512, NULL, NULL);
    handle_error(err);

    err = Pa_StartStream(stream);
    handle_error(err);
}

AudioEngine::~AudioEngine()
{
    for (unsigned int i = 0; i < sounds.size(); i++) {
        delete sounds[i];
    }

    PaError err = Pa_StopStream(stream);
    handle_error(err);

    err = Pa_CloseStream(stream);
    handle_error(err);

    err = Pa_Terminate();
    handle_error(err);    
}

Sequencer& AudioEngine::getSequencer()
{
    return sequencer;
}

SampleBank& AudioEngine::getBank()
{
    return bank;
}

void AudioEngine::addSound(Sound* sound)
{
    sounds.push_back(sound);
}

void AudioEngine::addSound(std::string path)
{
    sounds.push_back(new Sound(bank.get(path)));
}

void AudioEngine::update()
{
    PaTime current_time = Pa_GetStreamTime(stream);

    if (sequencer.update(current_time)) {
        auto instruments = sequencer.getCurrent();
        for (unsigned int i = 0; i < 9; i++) {
            if (instruments.test(i)) {
                sounds.push_back(new Sound(bank.get(std::string("samples/") + std::to_string(i))));
            }
        }
    }
    
    signed long writable = Pa_GetStreamWriteAvailable(stream);
    if (writable >= 256) {
        writeToStream(writable);
    }

    for (unsigned int i = 0; i < sounds.size(); i++) {
        if (sounds[i]->offset >= sounds[i]->sample->sampleCount) {
            delete sounds[i];
            sounds.erase(sounds.begin() + i);
        }
    }
}

void AudioEngine::writeToStream(signed long framesNum)
{
    float* out = static_cast<float*>(malloc(framesNum * 2 * sizeof(float)));
    for(unsigned int i = 0; i < framesNum * 2; i += 2) {
        out[i] = 0.f;
        out[i+1] = 0.f;
        for (auto it : sounds) {
            if (it->offset < it->sample->sampleCount) {
                if (it->sample->channels == 1) {
                    out[i] += it->sample->samples[it->offset];
                    out[i+1] += it->sample->samples[it->offset];
                    it->offset++;
                } else if (it->sample->channels == 2) {
                    out[i] += it->sample->samples[it->offset];
                    out[i+1] += it->sample->samples[it->offset+1];
                    it->offset += 2;
                }
            }
        }
        
        if (out[i] < -1.f) {
            out[i] = -1.f;
        } else if (out[i] > 1.f) {
            out[i] = 1.f;
        }

        if (out[i+1] < -1.f) {
            out[i+1] = -1.f;
        } else if (out[i+1] > 1.f) {
            out[i+1] = 1.f;
        }
    }
    Pa_WriteStream(stream, out, framesNum);
    free(out);
}

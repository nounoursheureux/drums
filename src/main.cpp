#include "portaudio.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include "sound.hpp"
#include "midi.hpp"

struct CallbackData
{
    std::vector<Sound*> sounds;
};

static int patestCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    float* out = static_cast<float*>(outputBuffer);
    CallbackData* cbData = static_cast<CallbackData*>(userData);

    for(unsigned int i = 0; i < framesPerBuffer; i++) {
        out[0] = 0.f;
        out[1] = 0.f;
        for (auto it : cbData->sounds) {
            if (it->offset < it->sample->sampleCount) {
                if (it->sample->channels == 1) {
                    out[0] += it->sample->samples[it->offset];
                    out[1] += it->sample->samples[it->offset];
                    it->offset++;
                } else if (it->sample->channels == 2) {
                    out[0] += it->sample->samples[it->offset];
                    out[1] += it->sample->samples[it->offset+1];
                    it->offset += 2;
                }
            }

            if (out[0] < -1.f) {
                out[0] = -1.f;
            } else if (out[0] > 1.f) {
                out[0] = 1.f;
            }

            if (out[1] < -1.f) {
                out[1] = -1.f;
            } else if (out[1] > 1.f) {
                out[1] = 1.f;
            }
        }
        out += 2;
    }
    return 0;
}

void handle_error(PaError err)
{
    if (err != paNoError) {
        fputs(Pa_GetErrorText(err), stderr);
        exit(1);
    }
}

int main()
{
    PaError err = Pa_Initialize();
    handle_error(err);

    SampleBank bank;

    CallbackData* cbData = new CallbackData();
    cbData->sounds.reserve(10);

    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 512, patestCallback, cbData);
    handle_error(err);

    MidiInput midi;

    err = Pa_StartStream(stream);
    handle_error(err);

    while (true) {
        snd_seq_event_t* ev = midi.read();
        if (ev != nullptr) {
            if (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity) {
                std::cout << "note on: " << int(ev->data.note.note) << std::endl;
                cbData->sounds.push_back(new Sound(bank.get(static_cast<unsigned int>(ev->data.note.note))));
            }
        }

        for (unsigned int i = 0; i < cbData->sounds.size(); i++) {
            if (cbData->sounds[i]->offset >= cbData->sounds[i]->sample->sampleCount) {
                delete cbData->sounds[i];
                cbData->sounds.erase(cbData->sounds.begin() + i);
            }
        }
    } 
    
    err = Pa_StopStream(stream);
    handle_error(err);

    err = Pa_CloseStream(stream);
    handle_error(err);

    err = Pa_Terminate();
    handle_error(err);

    delete cbData;
}

#include "portaudio.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "sound.hpp"
#include "midi.hpp"

struct CallbackData
{
    Sound* sound;
};

static int patestCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    float* out = static_cast<float*>(outputBuffer);
    CallbackData* cbData = static_cast<CallbackData*>(userData);

    for(unsigned int i = 0; i < framesPerBuffer; i++) {
        if (cbData->sound->offset < cbData->sound->sample->sampleCount) {
            if (cbData->sound->sample->channels == 1) {
                *out++ = cbData->sound->sample->samples[cbData->sound->offset];
                *out++ = cbData->sound->sample->samples[cbData->sound->offset];
                cbData->sound->offset++;
            } else if (cbData->sound->sample->channels == 2) {
                *out++ = cbData->sound->sample->samples[cbData->sound->offset];
                *out++ = cbData->sound->sample->samples[cbData->sound->offset+1];
                cbData->sound->offset += 2;
            }
        } else {
            *out++ = 0.f;
            *out++ = 0.f;
        }
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

    SampleFile sample("samples/17");
    Sound sound(&sample);

    std::cout << "Sample Rate: " << sample.sampleRate << "\nChannels: " << sample.channels << std::endl;

    CallbackData* cbData = new CallbackData();
    cbData->sound = &sound;

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
                std::cout << "note on" << std::endl;
            } else if (ev->type == SND_SEQ_EVENT_NOTEOFF || ev->type == SND_SEQ_EVENT_NOTEON) {
                std::cout << "note off" << std::endl;
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

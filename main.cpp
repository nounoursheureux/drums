#include "portaudio.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

struct CallbackData
{
    float* samples;
    uint64_t sampleCount;
    uint64_t offset;
    unsigned int channels;
};

static float* data;

static int patestCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
    float* out = static_cast<float*>(outputBuffer);
    CallbackData* cbData = static_cast<CallbackData*>(userData);

    for(unsigned int i = 0; i < framesPerBuffer; i++) {
        if (cbData->offset < cbData->sampleCount) {
            if (cbData->channels == 1) {
                *out++ = cbData->samples[cbData->offset];
                *out++ = cbData->samples[cbData->offset];
                cbData->offset++;
            } else if (cbData->channels == 2) {
                *out++ = cbData->samples[cbData->offset];
                *out++ = cbData->samples[cbData->offset+1];
                cbData->offset += 2;
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

    unsigned int channels, sampleRate;
    uint64_t sampleCount;
    float* pSamples = drwav_open_and_read_file_f32("samples/17", &channels, &sampleRate, &sampleCount);
    if (pSamples == NULL) {
        fputs("Couldn't read WAV file", stderr);
        return 1;
    }

    std::cout << "Sample Rate: " << sampleRate << "\nChannels: " << channels << std::endl;

    CallbackData* cbData = new CallbackData();
    cbData->samples = pSamples;
    cbData->sampleCount = sampleCount;
    cbData->channels = channels;
    cbData->offset = 0;

    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 512, patestCallback, cbData);
    handle_error(err);

    err = Pa_StartStream(stream);
    handle_error(err);
    Pa_Sleep(1000);
    
    err = Pa_StopStream(stream);
    handle_error(err);

    err = Pa_CloseStream(stream);
    handle_error(err);

    err = Pa_Terminate();
    handle_error(err);

    delete cbData;
}

#include "portaudio.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include "sound.hpp"
#include "midi.hpp"

struct PlaybackData
{
    std::vector<Sound*> sounds;
};

void handle_error(PaError err)
{
    if (err != paNoError) {
        fputs(Pa_GetErrorText(err), stderr);
        exit(1);
    }
}

void writeToStream(PaStream* stream, signed long framesNum, PlaybackData* data)
{
    float* out = static_cast<float*>(malloc(framesNum * 2 * sizeof(float)));
    for(unsigned int i = 0; i < framesNum * 2; i += 2) {
        out[i] = 0.f;
        out[i+1] = 0.f;
        for (auto it : data->sounds) {
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

int main()
{
    PaError err = Pa_Initialize();
    handle_error(err);

    SampleBank bank;

    PlaybackData* cbData = new PlaybackData();
    cbData->sounds.reserve(10);

    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 512, NULL, cbData);
    handle_error(err);

    MidiInput midi;

    err = Pa_StartStream(stream);
    handle_error(err);

    while (true) {
        signed long writable = Pa_GetStreamWriteAvailable(stream);
        if (writable > 0) {
            writeToStream(stream, writable, cbData);
        }
        snd_seq_event_t* ev = midi.read();
        if (ev != nullptr) {
            if (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity) {
                std::cout << "note on: " << int(ev->data.note.note) << std::endl;
                unsigned int sample_id = ev->data.note.note % 9;
                cbData->sounds.push_back(new Sound(bank.get(sample_id)));
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

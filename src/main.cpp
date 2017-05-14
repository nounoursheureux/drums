#include "portaudio.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include "sound.hpp"
#include "midi.hpp"
#include "sequencer.hpp"

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

    Sequencer sequencer;
    sequencer.set(0, 1, 1);
    sequencer.set(4, 1, 1);
    sequencer.set(8, 1, 1);
    sequencer.set(12, 1, 1);
    sequencer.set(2, 2, 1);
    sequencer.set(6, 2, 1);
    sequencer.set(10, 2, 1);
    sequencer.set(14, 2, 1);

    sequencer.set(0, 7, 1);
    sequencer.set(1, 7, 1);
    sequencer.set(2, 7, 1);
    sequencer.set(3, 7, 1);
    sequencer.set(4, 7, 1);
    sequencer.set(5, 7, 1);
    sequencer.set(6, 7, 1);
    sequencer.set(7, 7, 1);
    sequencer.set(8, 7, 1);
    sequencer.set(9, 7, 1);
    sequencer.set(10, 7, 1);
    sequencer.set(11, 7, 1);
    sequencer.set(12, 7, 1);
    sequencer.set(13, 7, 1);
    sequencer.set(14, 7, 1);
    sequencer.set(15, 7, 1);

    PlaybackData* cbData = new PlaybackData();
    cbData->sounds.reserve(10);

    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 512, NULL, NULL);
    handle_error(err);

    MidiInput midi;

    err = Pa_StartStream(stream);
    handle_error(err);

    PaTime latest_time = Pa_GetStreamTime(stream);
    PaTime elapsed_time = 0.0;
    double bpm = 140.0;
    double bps = bpm / 60.0;
    double time_per_beat = 1.0 / bps;
    double time_per_quarter_beat = time_per_beat / 4.0;

    unsigned int beat_number = 0;

    while (true) {
        PaTime latestest_time = Pa_GetStreamTime(stream);
        elapsed_time += latestest_time - latest_time;
        latest_time = latestest_time;

        if (elapsed_time >= time_per_quarter_beat) {
            auto instruments = sequencer.get(beat_number);
            for (unsigned int i = 0; i < 9; i++) {
                if (instruments.test(i)) {
                    cbData->sounds.push_back(new Sound(bank.get(i)));
                }
            }
            elapsed_time -= time_per_quarter_beat;
            beat_number = (beat_number + 1) % 16;
        }
        signed long writable = Pa_GetStreamWriteAvailable(stream);
        if (writable >= 256) {
            writeToStream(stream, writable, cbData);
        }
        snd_seq_event_t* ev = midi.read();
        if (ev != nullptr) {
            if (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity) {
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

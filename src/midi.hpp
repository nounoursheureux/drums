#ifndef MIDI_HPP
#define MIDI_HPP
#include <alsa/asoundlib.h>

class MidiInput
{
public:
    MidiInput();
    ~MidiInput();
    snd_seq_event_t* read();

private:
    snd_seq_t* seq_handle;
    int in_port;
};
#endif

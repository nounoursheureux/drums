#include "portaudio.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "sound.hpp"
#include "midi.hpp"
#include "sequencer.hpp"
#include "engine.hpp"
#include "arduino.hpp"

int main()
{
    AudioEngine audio;

    audio.getSequencer().set(0, 1, 1);
    audio.getSequencer().set(4, 1, 1);
    audio.getSequencer().set(8, 1, 1);
    audio.getSequencer().set(12, 1, 1);
    audio.getSequencer().set(2, 2, 1);
    audio.getSequencer().set(6, 2, 1);
    audio.getSequencer().set(10, 2, 1);
    audio.getSequencer().set(14, 2, 1);

    audio.getSequencer().set(0, 7, 1);
    audio.getSequencer().set(1, 7, 1);
    audio.getSequencer().set(2, 7, 1);
    audio.getSequencer().set(3, 7, 1);
    audio.getSequencer().set(4, 7, 1);
    audio.getSequencer().set(5, 7, 1);
    audio.getSequencer().set(6, 7, 1);
    audio.getSequencer().set(7, 7, 1);
    audio.getSequencer().set(8, 7, 1);
    audio.getSequencer().set(9, 7, 1);
    audio.getSequencer().set(10, 7, 1);
    audio.getSequencer().set(11, 7, 1);
    audio.getSequencer().set(12, 7, 1);
    audio.getSequencer().set(13, 7, 1);
    audio.getSequencer().set(14, 7, 1);
    audio.getSequencer().set(15, 7, 1);

    MidiInput midi;

    Arduino arduino;

    while (true) {
        audio.update();

        snd_seq_event_t* ev = midi.read();
        if (ev != nullptr) {
            if (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity) {
                unsigned int sample_id = ev->data.note.note % 9;
                std::string sample_path = std::string("samples/") + std::to_string(sample_id);
                audio.addSound(sample_path);
            }
        }
        if (arduino.isMessageAvailable()) {
            std::cout << "available" << std::endl;
            Message msg = arduino.readMessage();
            if (msg.type == MSGTYPE_READY && msg.arg1 == 0 && msg.arg2 == 0 && msg.arg3 == 0) {
                // set bpm
                Message msg(MSGTYPE_SET_BPM, 140, 0, 0);
                arduino.writeMessage(msg);
            }
        }
    }
}

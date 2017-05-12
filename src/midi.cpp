#include "midi.hpp"

MidiInput::MidiInput()
{
    snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, SND_SEQ_NONBLOCK);
    snd_seq_set_client_name(seq_handle, "drums");
    in_port = snd_seq_create_simple_port(seq_handle, "listen:in", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION);

    snd_seq_addr_t src, dest;
    snd_seq_port_subscribe_t* subs;
    src.client = 20;
    src.port = 0;
    dest.client = 128;
    dest.port = 0;
    snd_seq_port_subscribe_alloca(&subs);
    snd_seq_port_subscribe_set_sender(subs, &src);
    snd_seq_port_subscribe_set_dest(subs, &dest);
    snd_seq_port_subscribe_set_queue(subs, 0);
    snd_seq_port_subscribe_set_exclusive(subs, 0);
    snd_seq_port_subscribe_set_time_update(subs, 0);
    snd_seq_port_subscribe_set_time_real(subs, 0);
    snd_seq_subscribe_port(seq_handle, subs);
}

MidiInput::~MidiInput()
{
    snd_seq_close(seq_handle);
}

snd_seq_event_t* MidiInput::read()
{
    snd_seq_event_t* ev = nullptr;
    snd_seq_event_input(seq_handle, &ev);
    return ev;
}

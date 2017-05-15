#include "sequencer.hpp"

Sequencer::Sequencer(double bpm)
{
    setBpm(bpm);
    latest_time = 0.0;
    elapsed_time = 0.0;
    current_quarter_beat = 0;
}

void Sequencer::set(unsigned int quarterBeat, unsigned int instrument, bool val)
{
    beats.at(quarterBeat).set(instrument, val);
}

void Sequencer::setBpm(double bpm)
{
    double bps = bpm / 60.0;
    double time_per_beat = 1.0 / bps;
    time_per_quarter_beat = time_per_beat / 4.0;
}

std::bitset<9> Sequencer::get(unsigned int quarterBeat)
{
    return beats.at(quarterBeat);
}

std::bitset<9> Sequencer::getCurrent()
{
    return beats.at(current_quarter_beat);
}

bool Sequencer::update(double current_time)
{
    elapsed_time += current_time - latest_time;
    latest_time = current_time;
    if (elapsed_time >= time_per_quarter_beat)
    {
        current_quarter_beat = (current_quarter_beat + 1) % 16;
        // elapsed_time -= time_per_quarter_beat;
        elapsed_time = 0;
        return true;
    }

    return false;
}

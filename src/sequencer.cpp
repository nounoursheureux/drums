#include "sequencer.hpp"

Sequencer::Sequencer()
{
}

void Sequencer::set(unsigned int quarterBeat, unsigned int instrument, bool val)
{
    beats.at(quarterBeat).set(instrument, val);
}

std::bitset<9> Sequencer::get(unsigned int quarterBeat)
{
    return beats.at(quarterBeat);
}

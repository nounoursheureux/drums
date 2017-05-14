#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP
#include <bitset>
#include <array>

class Sequencer
{
public:
    Sequencer();
    void set(unsigned int quarterBeat, unsigned int instrument, bool val);
    std::bitset<9> get(unsigned int quarterBeat);

private:
    std::array<std::bitset<9>, 16> beats;
};
#endif

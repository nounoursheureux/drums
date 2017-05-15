#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP
#include <bitset>
#include <array>
#include <map>
#include <string>

class Sequencer
{
public:
    explicit Sequencer(double bpm);
    void set(unsigned int quarterBeat, unsigned int instrument, bool val);
    void setBpm(double bpm);
    std::bitset<9> get(unsigned int quarterBeat);
    std::bitset<9> getCurrent();
    bool update(double current_time);

private:
    std::array<std::bitset<9>, 16> beats;
    double latest_time, elapsed_time;
    unsigned int current_quarter_beat;
    double time_per_quarter_beat;
};
#endif

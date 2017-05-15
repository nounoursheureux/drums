#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP
#include <array>
#include <map>
#include <string>
#include <vector>

class Sequencer
{
public:
    explicit Sequencer(unsigned int tracksNum, double bpm);
    void set(unsigned int quarterBeat, unsigned int instrument, bool val);
    void setBpm(double bpm);
    std::vector<bool> get(unsigned int quarterBeat);
    std::vector<bool> getCurrent();
    bool update(double current_time);

private:
    std::array<std::vector<bool>, 16> tracks;
    double latest_time, elapsed_time;
    unsigned int current_quarter_beat;
    double time_per_quarter_beat;
};
#endif

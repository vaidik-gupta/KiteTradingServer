#ifndef RANDOM_TICK_SIMULATOR_H
#define RANDOM_TICK_SIMULATOR_H

#include <atomic>
#include <thread>
#include <cstdint>

class RandomTickSimulator {
private:
    std::atomic<double> value;
    double min_tick_diff;
    double max_tick_diff;
    std::atomic<uint64_t> timestamp;
    uint64_t min_tick_interval;
    uint64_t max_tick_interval;
    std::atomic<bool> stop_flag;
    std::thread ticker_thread;

public:
    RandomTickSimulator(
        double initialValue,
        double min_tick_diff = 0.01,
        double max_tick_diff = 0.1,
        uint64_t min_tick_interval = 10,
        uint64_t max_tick_interval = 100
    );
    ~RandomTickSimulator();
    void start();
    void stop();
    double getValue() const;
    uint64_t getTimestamp() const;
};

#endif // RANDOM_TICK_SIMULATOR_H

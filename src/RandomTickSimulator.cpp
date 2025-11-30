#include <random>
#include <chrono>
#include <atomic>
#include <thread>
#include <TickSimulators.h>

 

RandomTickSimulator::RandomTickSimulator(
    double initialValue,
    double min_tick_diff,
    double max_tick_diff,
    uint64_t min_tick_interval,
    uint64_t max_tick_interval)
    : min_tick_diff(min_tick_diff),
      max_tick_diff(max_tick_diff),
      min_tick_interval(min_tick_interval),
      max_tick_interval(max_tick_interval)
{
    stop_flag = true;
    value = initialValue;
}

RandomTickSimulator::~RandomTickSimulator() {
    stop();
}

void RandomTickSimulator::start() {

    ticker_thread = std::thread([this]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis_diff(min_tick_diff, max_tick_diff);
        std::uniform_int_distribution<> dis_interval(min_tick_interval, max_tick_interval);
        std::uniform_int_distribution<> dis_direction(0, 1); // 0 for down, 1 for up
        while (!stop_flag) {
            double tick_diff = dis_diff(gen);
            int direction = dis_direction(gen);
            double old_value = value.load();
            if(direction == 0) {
                old_value -= tick_diff;
            } else {
                old_value += tick_diff;
            }

            value.store(old_value);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(dis_interval(gen)));
        }
    });
    stop_flag = false;
}

void RandomTickSimulator::stop() {
    stop_flag = true;
    if (ticker_thread.joinable()) {
        ticker_thread.join();
    }
}

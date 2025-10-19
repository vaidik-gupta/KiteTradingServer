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
      max_tick_interval(max_tick_interval),
      stop_flag(false)
{
    value = initialValue;
    timestamp = 0;
    running = false;
}

RandomTickSimulator::~RandomTickSimulator() {
    stop();
}

void RandomTickSimulator::start() {
    stop_flag = false;
    ticker_thread = std::thread([this]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis_diff(min_tick_diff, max_tick_diff);
        std::uniform_int_distribution<> dis_interval(min_tick_interval, max_tick_interval);
        std::uniform_int_distribution<> dis_direction(0, 1); // 0 for down, 1 for up
        while (!stop_flag) {
            double tick_diff = dis_diff(gen);
            int direction = dis_direction(gen);
            if(direction == 0) {
                value -= tick_diff;
            } else {
                value += tick_diff;
            }
            timestamp += dis_interval(gen);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(dis_interval(gen)));
        }
    });
    running = true;
}

void RandomTickSimulator::stop() {
    stop_flag = true;
    if (ticker_thread.joinable()) {
        ticker_thread.join();
    }
    running = false;
}

double RandomTickSimulator::getValue() const {
    return value;
}

uint64_t RandomTickSimulator::getTimestamp() const {
    return timestamp;
}

bool RandomTickSimulator::isRunning() const {
    return running;
}

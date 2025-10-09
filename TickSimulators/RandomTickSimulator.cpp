#include <random>
#include <chrono>
#include <atomic>
#include <thread>
#include <TickSimulators.h>

 

RandomTickSimulator::RandomTickSimulator(
    double initialValue,
    double min_tick_diff = 0.01,
    double max_tick_diff = 0.1,
    uint64_t min_tick_interval = 10,
    uint64_t max_tick_interval = 100
)
    : value(initialValue),
        min_tick_diff(min_tick_diff),
        max_tick_diff(max_tick_diff),
        min_tick_interval(min_tick_interval),
        max_tick_interval(max_tick_interval),
        timestamp(0),
        stop_flag(false)
{
}

RandomTickSimulator::~RandomTickSimulator() {
    stop();
    if (ticker_thread.joinable()) {
        ticker_thread.join();
    }
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
}

void RandomTickSimulator::stop() {
        stop_flag = true;
    }

double RandomTickSimulator::getValue() const {
    return value;
    }

uint64_t RandomTickSimulator::getTimestamp() const {
    return timestamp;
}

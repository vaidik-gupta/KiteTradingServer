#ifndef TICK_SOURCE_H
#define TICK_SOURCE_H

#include <TimeSeriesBase.h>
#include <chrono>
#include <atomic>

class TickSource {
    protected:
        std::atomic<double> value;
        std::atomic<uint64_t> timestamp;
        std::atomic<bool> running;
    public:
        virtual ~TickSource() = default;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual bool isRunning() const = 0;
        virtual double getValue() const { return value.load(); }
};


#endif // TICK_SOURCE_H

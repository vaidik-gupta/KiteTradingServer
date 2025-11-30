#ifndef TICK_SOURCE_H
#define TICK_SOURCE_H
#include <chrono>
#include <atomic>

class TickSource {
    protected:
        std::atomic<double> value;
        std::atomic<bool> stop_flag;
    public:
        virtual ~TickSource() = default;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual bool isRunning() const{
            return !stop_flag.load();
        };
        virtual double getValue() const { return value.load(); }
        virtual void setValue(double val){
            value = val;
        }
};


#endif // TICK_SOURCE_H

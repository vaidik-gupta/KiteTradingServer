#include <TimeSeriesBase.h>
#include <chrono>
#include <list>
#include <memory>
#include <thread>
#include <TickSource.h>

typedef struct ts{
    std::unique_ptr<TimeSeriesBase> ts;
    std::shared_ptr<TickSource> source;
    const uint64_t interval;
    std::thread worker;
    std::atomic<bool> running;
} TimeSeries;

class BufferManager {
    private:
    std::list<TimeSeries> time_series_list;
    void startTimeSeries(std::list<TimeSeries>::iterator it);
    std::mutex list_mutex;
    public:
    BufferManager() = default;
    ~BufferManager();
    //  returns the iterator to the newly added TimeSeries else nullptr
    std::list<TimeSeries>::iterator addTimeSeries(std::unique_ptr<TimeSeriesBase> ts, std::shared_ptr<TickSource> source, uint64_t interval);

    void removeTimeSeries(std::list<TimeSeries>::iterator it);
};
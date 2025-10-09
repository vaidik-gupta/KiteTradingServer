#ifndef TIME_SERIES_BASE_H
#define TIME_SERIES_BASE_H

#include <deque>
#include <string>
#include <shared_mutex>
#include <mutex>

struct MetricData {
    double value;
    uint64_t flag;
};

using MetricFunc = MetricData (*)(const std::deque<double>&, double new_data, MetricData*, size_t);

struct MetricMetadata {
    std::string name;
    MetricFunc func;
};

class TimeSeriesBase {
private:
    std::deque<double> data;
    size_t max_size;
    size_t metrics_size;
    MetricData * metrics;
    MetricMetadata* metric_metadatas;
    mutable std::shared_mutex metrics_mutex;

protected:
    virtual void compute_metrics(double new_value);

public:
    TimeSeriesBase(size_t max_size, size_t metrics_size);
    virtual ~TimeSeriesBase();
    virtual void add_data(double value);
    virtual MetricData * get_metrics();
};

#endif // TIME_SERIES_BASE_H

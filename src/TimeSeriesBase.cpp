#include <TimeSeriesBase.h>



void TimeSeriesBase::compute_metrics(double new_value)
{
    std::unique_lock lock(metrics_mutex);
    MetricData new_metrics[metrics_size];
    for (size_t i = 0; i < metrics_size; ++i)
    {
        if (metric_metadatas[i].func != nullptr)
        {
            new_metrics[i] = metric_metadatas[i].func(data,new_value, metrics, metrics_size);
        }
    }
    std::copy(new_metrics, new_metrics + metrics_size, metrics);
}

TimeSeriesBase::TimeSeriesBase(size_t max_size, size_t metrics_size)
    : max_size(max_size), metrics_size(metrics_size)
{
    metrics = new MetricData[metrics_size];
    metric_metadatas = new MetricMetadata[metrics_size]();
    for (size_t i = 0; i < metrics_size; ++i)
    {
        metrics[i] = {0.0, 0};
        metric_metadatas[i] = {"", nullptr};
    }
}

TimeSeriesBase::~TimeSeriesBase()
{
    delete[] metrics;
    delete[] metric_metadatas;
}

void TimeSeriesBase::add_data(double value)
{
    compute_metrics(value);
    if (data.size() >= max_size)
    {
        data.pop_front();
    }
    data.push_back(value);
}

MetricData *TimeSeriesBase::get_metrics()
{
    std::shared_lock lock(metrics_mutex);
    MetricData *metrics_copy = new MetricData[metrics_size];
    std::copy(metrics, metrics + metrics_size, metrics_copy);
    return metrics_copy;
}
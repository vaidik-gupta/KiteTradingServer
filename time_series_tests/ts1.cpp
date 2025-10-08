#include <time_series_base.h>

// MetricData ema_func(const std::deque<double>& data, double new_data, MetricData* previous_metrics, size_t metrics_size){
//     const double alpha = 0.1; // Smoothing factor
//     double previous_ema = (metrics_size > 0) ? previous_metrics[1].value : new_data; // Assuming EMA is at index 1
//     double ema = (new_data * alpha) + (previous_ema * (1 - alpha));
//     return {ema, 0};
// }


// Factory function to create a TimeSeriesBase instance with predefined metrics of EMA, LTP, Range High
TimeSeriesBase * create_time_series(size_t max_size)
{
    TimeSeriesBase * ts = new TimeSeriesBase(max_size, 3);
    uint32_t ema_index = 1;
    uint32_t ltp_index = 0;
    uint32_t range_high_index = 2;

}
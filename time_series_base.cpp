#include <deque>

#include <pybind11/pybind11.h>

namespace py = pybind11;

typedef double (*MetricFunc)(const std::deque<double>&, double*, size_t);


class TimeSeriesBase {
    public:
    std::deque<double> data;
    size_t max_size;
    double * metrics;
    size_t metrics_size;
    MetricFunc* metric_funcs;


    mutable std::shared_mutex metrics_mutex;

    TimeSeriesBase(size_t max_size, size_t metrics_size)
        : max_size(max_size), metrics_size(metrics_size) {
        metrics = new double[metrics_size];
        metric_funcs = new MetricFunc[metrics_size];
    }


    virtual ~TimeSeriesBase() {
        delete[] metrics;
        delete[] metric_funcs;
    }

    virtual void add_data(double value) {
        if (data.size() >= max_size) {
            data.pop_front();            
        }
        data.push_back(value);
        compute_metrics();
    }

    virtual double * get_metrics() {
        std::shared_lock lock(metrics_mutex);
        double * metrics_copy = new double[metrics_size];
        std::copy(metrics, metrics + metrics_size, metrics_copy);
        return metrics_copy;
    }


    
    private:


    void compute_metrics() {
        std::unique_lock lock(metrics_mutex);
        double new_metrics[metrics_size];
        for (size_t i = 0; i < metrics_size; ++i) {
            if (metric_funcs[i]) {
                new_metrics[i] = metric_funcs[i](data, &metrics[i], max_size);
            }
        }
        std::copy(std::begin(new_metrics), std::end(new_metrics), metrics);
    }

};
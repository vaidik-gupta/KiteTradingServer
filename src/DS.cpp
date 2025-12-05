#include <MetricFuncs.h>

MetricDs::~MetricDs() {}

HighLowDs::HighLowDs(const uint64_t window, const bool low) : window(window), low(low), current_index(0) {}

double HighLowDs::update(const std::deque<double>& data, double new_data, MetricData* metrics, size_t metric_size, MetricFuncArgs * args){

    uint64_t size = data.size();


    if (!dq.empty() && dq.front() + window <= current_index) {
        dq.pop_front();
    }

    while (!dq.empty() && ((data[size + dq.back() - current_index] <= new_data) == !low)) {
        dq.pop_back();
    }

    dq.push_back(current_index);

    current_index++;

    if(dq.front() == (current_index - 1)) return new_data;
    return data[size + dq.front() + 1 - current_index];

}
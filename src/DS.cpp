#include <MetricFuncs.h>

MetricDs::~MetricDs() {}

HighLowDs::HighLowDs(const uint64_t window, const bool low) : window(window), low(low), current_index(0) {}

double HighLowDs::update(const std::deque<double>& data, double new_data, MetricData* metrics, size_t metric_size, MetricFuncArgs * args){

    uint64_t size = data.size();

    if (!dq.empty() && dq.front() <= current_index - window) {
        dq.pop_front();
    }

    while (!dq.empty() && (low ^ (data[size - current_index + dq.back()] <= new_data))) {
        dq.pop_back();
    }

    dq.push_back(current_index);  
    
    if(dq.front() == current_index++) return new_data;

    return data[size - current_index + dq.front()];

}
#pragma once

#include <MetricFuncArgs.h>
#include <TimeSeriesBase.h>
#include <deque>

#define FUNC_PTR(name)  &Func_##name
#define MAKE_ARGS(name, ...) new MetricFuncArgs_##name(__VA_ARGS__)
#define MAKE_NAME(name) #name
#define MAKE_DS(name, ...) MetricDs_##name(__VA_ARGS__)


#define MAKE_METADATA(name, ...) \
{ \
    .name = MAKE_NAME(name), \
    .func = FUNC_PTR(name), \
    .args = MAKE_ARGS(name, __VA_ARGS__) \
}

#define DS_UPDATE(data, new_data, m_data, size, args, ds) \
    ((ds) != nullptr) ? \
        (ds)->update(data, new_data, m_data, size, args) : \
        0.0 

// To add new metrics first define a macro/function with appropriate name that returns the ds pointer given args also
//  initialize a class as a header for the DS needed to implement in ds file

#define MetricDs_ltp() nullptr
MetricData Func_ltp(const std::deque<double>& data, double new_data, MetricData* m_data, size_t size, MetricFuncArgs * args, MetricDs * ds){
    return {new_data, 1};
}



class HighLowDs: public MetricDs{
    private:
    std::deque<double> dq;
    const uint64_t window;
    const bool low;
    uint64_t current_index;
    public:

    HighLowDs(const uint64_t window, const bool low);
    double update(const std::deque<double>& data, double new_data, MetricData* metrics, size_t metric_size, MetricFuncArgs * args);
};

#define MetricDs_range_high(window) new HighLowDs(window,0)  
MetricData Func_range_high(const std::deque<double>& data, double new_data, MetricData* m_data, size_t size, MetricFuncArgs * args, MetricDs * ds){

    double metric = DS_UPDATE(data,new_data,m_data,size,args,ds);

    WindowArg * args2 = dynamic_cast<WindowArg *>(args);

    if(data.size() < args2->window) return {0.0,0};
    return {metric, 1};
}

#define MetricDs_range_low(window) new HighLowDs(window,1)  
MetricData Func_range_low(const std::deque<double>& data, double new_data, MetricData* m_data, size_t size, MetricFuncArgs * args, MetricDs * ds){

    double metric = DS_UPDATE(data,new_data,m_data,size,args,ds);
    WindowArg * args2 = dynamic_cast<WindowArg *>(args);

    if(data.size() < args2->window) return {0.0,0};
    return {metric, 1};
}


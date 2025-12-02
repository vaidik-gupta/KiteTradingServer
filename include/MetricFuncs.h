#include <MetricFuncArgs.h>
#include <TimeSeriesBase.h>


#define FUNC_PTR(name)  &Func_##name
#define MAKE_ARGS(name, ...) new MetricFuncArgs_##name(__VA_ARGS__)
#define MAKE_NAME(name) #name


#define MAKE_METADATA(name, ...) \
{ \
    .name = MAKE_NAME(name), \
    .func_ptr = FUNC_PTR(name), \
    .args = MAKE_ARGS(name, __VA_ARGS__) \
}





MetricData Func_ltp(const std::deque<double>& data, double new_data, MetricData* m_data, size_t size, MetricFuncArgs * args){
    return {new_data, 1};
}

// MetricData Func_range_high(const std::deque<double>& data, double new_data, MetricData* m_data, size_t size, MetricFuncArgs * args){
    
// }

#pragma once

#include <cstdint>

class MetricFuncArgs{
    public:
        uint32_t metric_index;
        MetricFuncArgs(uint32_t mi): metric_index(mi) {} 
        virtual ~MetricFuncArgs() = default;
};


class WindowArg: public MetricFuncArgs{
    public:
    uint64_t window;
    WindowArg(uint32_t mi, uint64_t window): MetricFuncArgs(mi), window(window) {}
};
using MetricFuncArgs_range_high = WindowArg;
using MetricFuncArgs_range_low = WindowArg;
using MetricFuncArgs_sma = WindowArg;


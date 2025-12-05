#pragma once

#include <cstdint>

class MetricFuncArgs{
    public:
        MetricFuncArgs() = default;
        virtual ~MetricFuncArgs() = default;
};


class WindowArg: public MetricFuncArgs{
    public:
    uint64_t window;
    WindowArg(uint64_t window): window(window) {}
};
using MetricFuncArgs_range_high = WindowArg;
using MetricFuncArgs_range_low = WindowArg;
using MetricFuncArgs_sma = WindowArg;
using MetricFuncArgs_ltp = MetricFuncArgs;


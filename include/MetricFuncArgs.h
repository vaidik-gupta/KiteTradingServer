#include <cstdint>

class MetricFuncArgs{
    public:
        uint32_t metric_index;
        MetricFuncArgs(uint32_t mi): metric_index(mi) {} 
};


class smaArgs : public MetricFuncArgs{
    public:
        uint32_t interval;
        smaArgs(uint32_t mi, uint32_t ivl): MetricFuncArgs(mi), interval(ivl) {}
};


class emaArgs : public smaArgs{
    public:
        double decay_factor;
        emaArgs(uint32_t mi, uint32_t ivl, double df): smaArgs(mi,ivl), decay_factor(df) {}
};

#define rangeHighArgs smaArgs
#define rangeLowArgs smaArgs

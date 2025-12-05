#include <MetricFuncArgs.h>
#include <MetricFuncs.h>
#include <TimeSeriesBase.h>

#include <random>
#include <deque>
#include <cassert>
#include <iostream>

int main(){
    // Part 1: API sanity check for ltp metric
    {
        TimeSeriesBase ts(10,1);
        ts.metric_metadatas[0] = MAKE_METADATA(ltp);
        ts.dss[0] = MAKE_DS(ltp);

        ts.add_data(3.1415);
        MetricData *m = ts.get_metrics();
        assert(m[0].value == 3.1415);
        delete[] m;

        ts.add_data(2.71828);
        m = ts.get_metrics();
        assert(m[0].value == 2.71828);
        delete[] m;

        std::cout << "API ltp check passed." << std::endl;
    }

    // Part 2: Large time-series test for HighLowDs (range_high)
    {
        const size_t window = 1000;
        const size_t total = 50000; // large time series
        const size_t size = 5000;
        const uint32_t seed = 42; // reproducible

        TimeSeriesBase ts(size, 1);
        ts.metric_metadatas[0] = MAKE_METADATA(range_high,window);
        ts.dss[0] = MAKE_DS(range_high,window);

        // add sanity checks here wheter the computed metric matches expected
        assert(ts.dss[0] != nullptr);
        auto meta = ts.metric_metadatas[0];
        assert(meta->name == "range_high");
        assert(meta->func == FUNC_PTR(range_high));
        auto args = dynamic_cast<WindowArg*>(ts.metric_metadatas[0]->args);
        assert(args != nullptr);
        assert(args->window == window);

        
        std::mt19937 rng(seed);
        std::normal_distribution<double> delta_dist(0.0, 0.2); // delta distribution: mean=0, std=0.2

        std::deque<double> reference; // mirror of TimeSeriesBase data
        double current_price = 500.0; // starting price

        for(size_t i = 0; i < total; ++i){
            double delta = delta_dist(rng);
            current_price += delta;

            // compute expected maximum over last `window` values including current_price
            std::deque<double> tmp = reference;
            tmp.push_back(current_price);
            if(tmp.size() > window){
                while(tmp.size() > window) tmp.pop_front();
            }

            double expected = 0.0;
            if(tmp.size() == 0) expected = 0.0;
            else {
                expected = tmp.front();
                for(double x: tmp) if(x > expected) expected = x;
            }

            // add to timeseries (this triggers metric computation)
            ts.add_data(current_price);

            // keep reference in sync with TimeSeriesBase push/pop
            reference.push_back(current_price);
            if(reference.size() > window + 100) reference.pop_front(); // safe-guard, though max_size is large

            // retrieve metrics and check after we've accumulated at least `window` values
            MetricData *m = ts.get_metrics();
            if(i + 1 >= window){
                double got = m[0].value;
                double eps = 1e-9;
                if(std::abs(got - expected) > eps){
                    std::cerr << "Mismatch at index " << i << ": got=" << got << " expected=" << expected << std::endl;
                    delete[] m;
                    return 1;
                }
            }
            delete[] m;
        }

        std::cout << "HighLowDs range_high large-series test passed." << std::endl;
    }

    return 0;
}

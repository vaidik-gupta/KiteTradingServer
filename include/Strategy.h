#ifdef STRATEGY_H
#define STRATEGY_H
#include <BufferManager.h>
#include <list>



using TSIter = std::list<TimeSeries>::iterator;
using TransFunc = uint32_t (*)(uint32_t n_ts, TSIter* time_series_list);



class Strategy{
    protected:
        uint32_t n_states;
        uint32_t n_ts;
        uint32_t current_state;        
        std::list<TimeSeries>::iterator * time_series_list;
        TransFunc * transition_functions;
    public:
        Strategy(uint32_t n_states, uint32_t n_ts, std::list<TimeSeries>::iterator * time_series_list, TransFunc * transition_functions)
            : n_states(n_states), n_ts(n_ts), current_state(0), time_series_list(time_series_list), transition_functions(transition_functions) {}

        virtual uint32_t updateState(){
            current_state = transition_functions[current_state](n_ts, time_series_list);
            return current_state;
        } 
        virtual ~Strategy() = default;
};

#endif // STRATEGY_H
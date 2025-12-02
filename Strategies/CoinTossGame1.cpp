#include <SocketSource.h>
#include <Strategy.h>
#include <BufferManager.h>
#include <MetricFuncs.h>

static uint32_t METRICSIZE = 3;



static TimeSeriesBase * getTS1(const uint32_t t_total){
    TimeSeriesBase *ts = new TimeSeriesBase(t_total,METRICSIZE);
    MetricMetadata * metadatas = ts->metric_metadatas;


    metadatas[0].name = "ltp";
    metadatas[0].args = MetricFuncArgs_ltp(0);
    metadatas[0].func = &Func_ltp;

    metadatas[0].name = "range_high";

    
}



Strategy * CoinTossGame1(const uint32_t t_inact, const uint32_t t_total){

    Strategy * S = new Strategy();


}
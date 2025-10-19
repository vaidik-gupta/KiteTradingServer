#include <TickSimulators.h>
#include <TickSource.h>


#include <assert.h>


signed main(){

    RandomTickSimulator rts(100.0);
    bool running = rts.isRunning();
    assert(running == false);
    rts.start();
    running = rts.isRunning();
    assert(running == true);
    // Lets visually inspect the value changes for the next 2 minutes
    for(int i = 0; i < 60; i++){
        double value = rts.getValue();
        uint64_t timestamp = rts.getTimestamp();
        printf("Value: %f, Timestamp: %lu\n", value, timestamp);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}


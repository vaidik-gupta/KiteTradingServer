#include <TickSimulators.h>
#include <TickSource.h>
#include <SocketSource.h>
#include <WebSockets.h>


#include <assert.h>
#include <cstdio>

#define BINANCEURL "wss://stream.binance.com:9443/ws/btcusdt@trade"


signed main(){

    RandomTickSimulator rts(100.0);
    bool running = rts.isRunning();
    assert(running == false);
    rts.start();
    running = rts.isRunning();
    assert(running == true);
    // Lets visually inspect the value changes for the next 2 minutes
    for(int i = 0; i < 10; i++){
        double value = rts.getValue();
        printf("Value: %f\n", value);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    rts.stop();
    running = rts.isRunning();
    assert(running == false);
    
    std::string url = BINANCEURL;
    SocketSource ss(url,&OnMessageBinSocket);

    running = ss.isRunning();

    assert(running == false);
    ss.start();
    running = ss.isRunning();
    assert(running == true);

    // Lets visually inspect the value changes for the next 2 minutes
    for(int i = 0; i < 10; i++){
        double value = ss.getValue();
        printf("Value: %f\n", value);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ss.stop();

}


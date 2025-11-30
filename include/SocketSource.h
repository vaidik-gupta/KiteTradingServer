#ifndef SOCKET_SOURCE_H
#define SOCKET_SOURCE_H


#include <TickSource.h>
#include <WebSockets.h>

class SocketSource: public TickSource{
    private:
    const std::string ws_url;
    handler hdl;
    client cl;
    OnMessageFunc1 func;
    std::thread ws_thread; 
    public:

    SocketSource(const std::string& url, OnMessageFunc1 func) : ws_url(url), func(func){
        stop_flag = true;
    }
    ~SocketSource(){
        stop();
    }
    void stop();
    void start();
};

#endif 



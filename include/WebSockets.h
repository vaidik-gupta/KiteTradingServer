#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <boost/asio/ssl.hpp>
#include <TickSource.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef websocketpp::connection_hdl handler;
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::error_code SocketError;
typedef  client::connection_ptr ConnectionPtr;


#define BINANCEURL "wss://stream.binance.com:9443/ws/btcusdt@trade"



using OnMessageFunc1 =  void (*) (TickSource *, websocketpp::connection_hdl, client::message_ptr);
// using OnMessageFunc2 = 

inline void OnMessageBinSocket(TickSource * ts, websocketpp::connection_hdl hdl, client::message_ptr msg){
    json j = json::parse(msg->get_payload());
    double price = std::stod(j["p"].get<std::string>());
    ts->setValue(price);
}

#endif




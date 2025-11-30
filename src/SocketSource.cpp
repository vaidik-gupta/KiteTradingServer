#include <SocketSource.h>
#include <boost/asio/ssl.hpp>


void SocketSource::stop(){
    cl.stop();
    stop_flag = true;
    if (ws_thread.joinable()) {
        ws_thread.join();
    }
}

void SocketSource::start(){
    cl.init_asio();


    cl.set_tls_init_handler([](websocketpp::connection_hdl) {
            // Use boost::asio::ssl::context explicitly and return the shared_ptr type expected by websocketpp
            auto ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12_client);
            ctx->set_options(boost::asio::ssl::context::default_workarounds);
            return ctx;
        });

    cl.set_message_handler(
    [this](handler hdl, client::message_ptr msg) {
        func(this,hdl,msg);
    }
    );

    
    cl.set_open_handler([this](handler hdl){
        this->hdl = hdl;
    });


    SocketError err;

    ConnectionPtr con = cl.get_connection(ws_url, err);

    if (err) {
        std::cerr << "Connect error: " << err.message() << std::endl;
        return;
    }

    cl.connect(con);



    ws_thread = std::thread([this]() {
        cl.run();      // <── event loop lives here
    });

    stop_flag = false;
}
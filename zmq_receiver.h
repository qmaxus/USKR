// zmq_receiver.h
#ifndef ZMQ_RECEIVER_H
#define ZMQ_RECEIVER_H

#include <zmq.hpp>
#include <string>
#include <nlohmann/json.hpp>

class ZmqReceiver {
public:
    ZmqReceiver(const std::string& config_file);
    ~ZmqReceiver();

    nlohmann::json receive();

private:
    zmq::context_t context_;
    zmq::socket_t socket_;
    std::string endpoint_;
};

#endif // ZMQ_RECEIVER_H
// zmq_sender.h
#ifndef ZMQ_SENDER_H
#define ZMQ_SENDER_H

#include <zmq.hpp>
#include <string>
#include <nlohmann/json.hpp>

class ZmqSender {
public:
    ZmqSender(const std::string& config_file);
    ~ZmqSender();

    void send(const void* data, size_t size);

private:
    zmq::context_t context_;
    zmq::socket_t socket_;
    std::string endpoint_;
};

#endif // ZMQ_SENDER_H

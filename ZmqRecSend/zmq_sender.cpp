// zmq_sender.cpp
#include "zmq_sender.h"

ZmqSender::ZmqSender(const std::string& config_file) {
    nlohmann::json config = nlohmann::json::parse(config_file);
    endpoint_ = config["zmq"]["endpoint"];
    context_ = zmq::context_t(1);
    socket_ = zmq::socket_t(context_, ZMQ_REQ);
    socket_.connect(endpoint_);
}

ZmqSender::~ZmqSender() {
    socket_.close();
    context_.close();
}

void ZmqSender::send(const void* data, size_t size) {
    zmq::message_t msg(size);
    memcpy(msg.data(), data, size);
    socket_.send(msg);
}
// zmq_receiver.cpp
#include "zmq_receiver.h"
#include <iostream>


ZmqReceiver::ZmqReceiver(const nlohmann::json& config) {
    endpoint_ = config["logic_sorted"]["1"]["ip_port_in"];
    context_ = zmq::context_t(1);
    socket_ = zmq::socket_t(context_, zmq::socket_type::pull);
    socket_.connect(endpoint_);
}

ZmqReceiver::~ZmqReceiver() {
    socket_.close();
    context_.close();
}

nlohmann::json ZmqReceiver::receive() {
    zmq::message_t msg;
    socket_.recv(msg);
    return nlohmann::json::parse(msg.to_string());
}
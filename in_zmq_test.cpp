#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <typeinfo>
//#include "ModelDso.h"
#include "ModelCam.h"
#include "Mediator.h"



int main() {
    std::string filename = "60000_";
    Camera& cam = Camera::getInstance();
    std::vector<std::vector<int>> dataAxle = load_file(filename);
    Sensor& dso = Sensor::getInstance();
    ConcreteMediator mediator(&cam, &dso);
    //nlohmann::json config = load_file("dataConfig.json");
   // std::cout<<config["shm"]<<std::endl;
    //add disconnect
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::pull);
    socket.connect("tcp://localhost:5555");
    zmq::message_t message;

    double unixTime;
    std::cout << std::fixed <<std::endl;

    while (true) {

        socket.recv(message);
        nlohmann::json jsonData = nlohmann::json::parse(message.to_string());
        unixTime = getTime();

        if (jsonData["info"]=="uskr_kgm"){
           //cam.updateJson(jsonData);
        }
        else if (jsonData["info"]=="uskr_dso"){
            dso.updateJson(jsonData);

        }
        else if (jsonData["info"]=="rco"){
           //rco.updateJson(jsonData);
        }
        else if (jsonData["info"]=="uskr_lasers"){
           //laser.updateJson(jsonData);
        }
        else if (typeid(jsonData["info"]) ==typeid(jsonData)){
              cam.updateJson(jsonData,  dataAxle);
             // cam.print();
              //add annotations

        }

        //send_diagnostic()
        std::cout << "ECHO is alive" <<std::endl;

    }

    return 0;
}

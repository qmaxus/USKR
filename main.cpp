#include <zmq.hpp>
#include <typeinfo>
#include "Camera/Camera.h"
#include "Sensor/Sensor.h"
#include "Circuit/Circuit.h"
#include "Laser/Laser.h"
#include "ConcreteMediator.h"
#include "ConfigLoader/ConfigLoader.h"
#include "ZmqRecSend/zmq_receiver.h"


int main() {
    std::cout << std::fixed <<std::endl;
    // Загрузка конфигурации
    nlohmann::json config = ConfigLoader::loadConfig("ConfigLoader/config.json");

    // Создаем объекты камеры, датчика, цепи и лазера
    Camera camera(&ConcreteMediator::getInstance(), "Camera1",config);
    Sensor sensor(&ConcreteMediator::getInstance(), "Sensor1");
    Circuit circuit(&ConcreteMediator::getInstance(), "Circuit1");
    Laser laser(&ConcreteMediator::getInstance(), "Laser1");

    // Устанавливаем компоненты в посредник
    ConcreteMediator::getInstance().setComponents(&camera, &sensor, &circuit, &laser);

    // Устанавливаем соединение по ZMQ
    ZmqReceiver receiver(config);


    while(true){
        nlohmann::json jsonData = receiver.receive();
       // std::cout<<jsonData<<std::endl;
        if (jsonData["info"]=="uskr_kgm"){
           //cam.updateJson(jsonData);
        }
        else if (jsonData["info"]=="uskr_dso")
            sensor.update(jsonData);  // Датчик срабатывает
        else if (jsonData["info"]=="rco")
            circuit.activate();
        else if (jsonData["info"]=="uskr_lasers")
            laser.fire();
         else if (jsonData["info"]=="rtds")
            laser.fire();
        else if (typeid(jsonData["info"]) ==typeid(jsonData))
            camera.update(jsonData);

        //send_diagnostic()
        //std::cout << "\t\tECHO is alive" <<std::endl;



    }
    return 0;
}
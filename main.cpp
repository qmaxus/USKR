#include <zmq.hpp>
#include <typeinfo>
#include "Camera.h"
#include "Sensor.h"
#include "Circuit.h"
#include "Laser.h"
#include "ConcreteMediator.h"
#include "ConfigLoader.h"
#include "zmq_receiver.h"


int main() {
    // Загрузка конфигурации
    std::string config = ConfigLoader::loadConfig("config.json");
    std::cout << "Loaded configuration: " << config << std::endl;

    // Создаем объекты камеры, датчика, цепи и лазера
    Camera camera(&ConcreteMediator::getInstance(), "Camera1");
    Sensor sensor(&ConcreteMediator::getInstance(), "Sensor1");
    Circuit circuit(&ConcreteMediator::getInstance(), "Circuit1");
    Laser laser(&ConcreteMediator::getInstance(), "Laser1");

    // Устанавливаем компоненты в посредник
    ConcreteMediator::getInstance().setComponents(&camera, &sensor, &circuit, &laser);

    // Устанавливаем соединение по ZMQ
    ZmqReceiver receiver(config);


    while(true){
        nlohmann::json jsonData = receiver.receive();

        if (jsonData["info"]=="uskr_kgm"){
           //cam.updateJson(jsonData);
        }
        else if (jsonData["info"]=="uskr_dso")
            sensor.update(jsonData);  // Датчик срабатывает
        else if (jsonData["info"]=="rco")
            circuit.activate();
        else if (jsonData["info"]=="uskr_lasers")
            laser.fire();
        else if (typeid(jsonData["info"]) ==typeid(jsonData))
            camera.update(jsonData);

        //send_diagnostic()
        std::cout << "ECHO is alive" <<std::endl;



    }
    return 0;
}
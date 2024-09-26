#ifndef CONCRETE_MEDIATOR_H
#define CONCRETE_MEDIATOR_H

#include "Mediator.h"
#include "Camera.h"
#include "Sensor.h"
#include "Circuit.h"
#include "Laser.h"

class ConcreteMediator : public Mediator {
private:
    Camera* camera;
    Sensor* sensor;
    Circuit* circuit;
    Laser* laser;

    // Приватный конструктор для реализации одиночки
    ConcreteMediator() : camera(nullptr), sensor(nullptr), circuit(nullptr), laser(nullptr) {}

public:
    // Метод для получения экземпляра одиночки
    static ConcreteMediator& getInstance() {
        static ConcreteMediator instance;
        return instance;
    }

    // Устанавливаем компоненты в посредник
    void setComponents(Camera* cam, Sensor* sen, Circuit* cir, Laser* las) {
        camera = cam;
        sensor = sen;
        circuit = cir;
        laser = las;
    }

    void notify(const std::string& sender, const std::string& event) override {
        if (event == "MotionDetected") {
            std::cout << "Mediator reacts to motion detection." << std::endl;
            camera->print();
            //sensor->triggerAlarm();
            //circuit->activate();
            //laser->fire();
        } else if (event == "AlarmTriggered") {
            std::cout << "Mediator reacts to alarm trigger." << std::endl;
            // Дополнительные действия при срабатывании тревоги
        } else if (event == "CircuitActivated") {
            std::cout << "Mediator reacts to circuit activation." << std::endl;
            // Дополнительные действия при активации цепи
        } else if (event == "LaserFired") {
            std::cout << "Mediator reacts to laser firing." << std::endl;
            // Дополнительные действия при стрельбе лазера
        }
    }
};

#endif // CONCRETE_MEDIATOR_H
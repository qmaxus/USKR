#ifndef MEDIATOR_H
#define MEDIATOR_H
#include "ModelDso.h"
#include "ModelCam.h"


// Интерфейс посредника
class Mediator {
public:
    virtual void notify(const std::string& sender, const std::string& event) = 0;
};





class ConcreteMediator : public Mediator {
private:
    Camera* camera;
    Sensor* sensor;
    //Laser* laser;
public:
    ConcreteMediator(Camera* cam, Sensor* sen) : camera(cam), sensor(sen) {};
    void notify(const std::string& sender, const std::string& event) override {
        if (event == "MotionDetected") {
            std::cout << "Mediator reacts to motion detection." << std::endl;
            camera->print();
            sensor->print();
        } else if (event == "AlarmTriggered") {
            std::cout << "Mediator reacts to alarm trigger." << std::endl;
            // Дополнительные действия при срабатывании тревоги
        }
}
};



#endif  // MEDIATOR_H
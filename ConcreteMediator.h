#ifndef CONCRETE_MEDIATOR_H
#define CONCRETE_MEDIATOR_H
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <chrono>
#include "Mediator.h"
#include "Camera.h"
#include "Sensor.h"
#include "Circuit.h"
#include "Laser.h"

double getTime(){
    auto now = std::chrono::high_resolution_clock::now();
    auto unixTime = static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()) +
                    static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() % 1000000) / 1000000.0;
    return unixTime;
 }

double DatatimeToUnixtime(std::string datetimeString){

    std::tm tm = {}; // Initialize tm structure
    std::istringstream iss(datetimeString);
    iss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S"); // Parse year, month, day, hour, minute, and second
    iss >> std::setw(3) >> tm.tm_sec; // Parse milliseconds

    std::time_t tt = mktime(&tm);
    uint64_t unixTime = static_cast<uint64_t>(tt);

    // Add the remaining milliseconds
    unixTime *= 1000; // Convert seconds to milliseconds
    unixTime += tm.tm_sec % 1000; // Add the remaining milliseconds
    return unixTime;

}






class ConcreteMediator : public Mediator {
private:
    Camera* camera;
    Sensor* sensor;
    Circuit* circuit;
    Laser* laser;
    double TimeLastSend;

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

    void makeWagon(){
        Data& data = sensor->getData();
        for (Interval& dsoCouple: data.ListIntervals){
            std::cout<<dsoCouple.StartDate<<std::endl;
            double unixTimeStart = DatatimeToUnixtime(dsoCouple.StartDate);
            double unixTimeEnd = DatatimeToUnixtime(dsoCouple.EndDate);
            std::cout<<"unixTimeStart "<<unixTimeStart<<std::endl;
            UskrData numberCurrent = camera->getNumber(1.2,1.2);
            std::cout<<numberCurrent.info<<" t: "<<numberCurrent.time<<std::endl;
            UskrData coupleCurrent = camera->getCouple(1.2,1.2);
            std::cout<<coupleCurrent.info<<" t: "<<coupleCurrent.time<<std::endl;
            UskrData markCurrent = camera->getMark(1.2,1.2);
            std::cout<<markCurrent.info<<" t: "<<markCurrent.time<<std::endl;

            std::cout<<dsoCouple.EndDate<<std::endl;

        }
    }

    void notify(const std::string& sender, const std::string& event) override {
        TimeLastSend=getTime();
        if (event == "CameraDetected") {
            //std::cout << "Mediator reacts to motion detection." << std::endl;
        } else if (event == "SensorWork") {
             std::cout<<"-----start model------------------"<<std::endl;
             //sensor->print();
              makeWagon();
             //camera->print();
             std::cout<<"-----end model------------------"<<std::endl;
        } else if (event == "CircuitActivated") {
           // std::cout << "Mediator reacts to circuit activation." << std::endl;
            // Дополнительные действия при активации цепи
        } else if (event == "LaserFired") {
            //std::cout << "Mediator reacts to laser firing." << std::endl;
            // Дополнительные действия при стрельбе лазера
        }
    }







};

#endif // CONCRETE_MEDIATOR_H
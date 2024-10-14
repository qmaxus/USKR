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

double stringToUnixTime(const std::string& str) {
    // Parse the string into a time_point
    std::tm tm = {};
    std::istringstream ss(str.substr(0, 19)); // Remove the fractional second part
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse time string");
    }
    // Convert the time_point to Unix time in seconds
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    // Add the fractional second part
    auto fractionalSeconds = std::stod(str.substr(20)); // Get the fractional second part
    double milliseconds = seconds  + fractionalSeconds/1000;
    return milliseconds;
}


struct Msg{
            /*"time":msg.time,
            "info": msg.info,
            "dct":msg.dct,
            "vel": msg.vel,
            "axle":msg.axle,
            "info_status":msg.info_status,
            "type": msg.typeModel,
            "type_vgn": msg.type_vgn,
            "cargoType": msg.cargoType,
            "inv":*/
            double time;
            int dct;
            int axle;
            int info_status;
            std::string typeModel;
            int type_vgn;
            int status;
            double TimeAxis;
            int NumAxis;
            double vel;
            std::string info;
            int cargoType;
            std::string inv;
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
    double getTimeCouple( std::vector<Interval>& ListIntervalCouple, int account, int QuantityCouple ){
        double endTime = stringToUnixTime(ListIntervalCouple[account].EndDate);
        if (account<QuantityCouple-1){
            double StartTimeNext = stringToUnixTime(ListIntervalCouple[account+1].StartDate);
            endTime = (endTime + StartTimeNext)/2;
        }
        else{
            endTime += 5;
        }
        return endTime;
    }




    void makeWagon(){
        Data& data = sensor->getData();
        int QuantityCouple = data.ListIntervals.size();
        std::cout<<"------------------NUmmesssage: "<<data.NumMessage<<"--------------------------\n";
        double TimeLastSend = 0.0;

        for (int i=0; i<QuantityCouple;i++){

            double startTime = stringToUnixTime(data.ListIntervals[i].StartDate);
            double endTime = stringToUnixTime(data.ListIntervals[i].EndDate);
            double endTimeNext = getTimeCouple(data.ListIntervals,i,QuantityCouple);
            std::cout<<"\tDSO timeStart: "<<startTime<<" EndTime: "<<endTime<<" NextTimer: "<<endTimeNext<<"\n";

            // std::cout<<startTime<<std::endl;
            std::map<std::string, UskrData> numberReturn;
            numberReturn = camera->getNumber(startTime, endTimeNext);
            std::vector<UskrData> coupleCurrent = camera->getCouple(startTime, endTimeNext);
            std::vector<UskrData> markCurrent = camera->getMark(startTime, endTimeNext);


             std::cout<<"\tDSO vagon: "<<"Raliabilty:"<<data.ListIntervals[i].Reliability<<" axle: "<<data.ListIntervals[i].CountAxis<<"\n";
             for (const auto& pair1 : numberReturn)
                {
                std::stringstream ss;
                for (const auto& pair : pair1.second.cameraId) {
                    ss << "Key: " << pair.first << ", Value: " << pair.second << " | ";
                }

                 std::string mapAsString = ss.str();
                 std::cout<<"\tNumber vagon: "<<pair1.second.info<<" probability: "<<pair1.second.probability<<" Time: "<<pair1.second.time<<"MINT: "<<pair1.second.timeMin<<" MAXT: "<<pair1.second.timeMax<<"ID cam: "<< mapAsString<<"\n";
             }
             for (const auto& couple : coupleCurrent)
                std::cout<<"\tCouple vagon: "<<couple.info<<" probability: "<<couple.probability<<" Time: "<<couple.time<<"\n";
             for (const auto& mark : markCurrent)
                std::cout<<"\tMark vagon: "<<mark.info<<" probability: "<<mark.probability<<" Time: "<<mark.time<<"\n";

             if (data.ListIntervals[i].Reliability==0){
                std::cout<<"\t\t>>>>>>>>>>>>Send vagon: "<<data.ListIntervals[i].Reliability<<" | "<<data.ListIntervals[i].EndDate<<std::endl;

                    msg = {"time":msg.time,
                           "info": msg.info,
                           "dct":msg.dct,
                           "vel": msg.vel,
                           "axle":msg.axle,
                           "info_status":msg.info_status,
                           "type": msg.typeModel,
                           "type_vgn": msg.type_vgn,
                           "cargoType": msg.cargoType,
                           "inv": msg.inv}
                    MyStruct myStruct = {1, "John", 3.14};
                    nlohmann::json json = myStruct;
             }
             else {





             }
             if (data.ListIntervals[i].IsLastVagon)
                 std::cout<<"----ENd--------------IsLastVagon: "<<data.ListIntervals[i].IsLastVagon<<"\n";
        }
    }

    void notify(const std::string& sender, const std::string& event) override {
        TimeLastSend=getTime();
        if (event == "CameraDetected") {
            //std::cout << "Mediator reacts to motion detection." << std::endl;
        } else if (event == "SensorWork") {
              makeWagon();
        } else if (event == "CircuitActivated") {
            // std::cout << "Mediator reacts to circuit activation." << std::endl;
        } else if (event == "LaserFired") {
            //std::cout << "Mediator reacts to laser firing." << std::endl;
        }
    }







};

#endif // CONCRETE_MEDIATOR_H



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
#include "Graf.h"

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

enum Reliability{
    Sure = 0,
    NotSure = 1,
    End  = 777,
    Empty =666
};

enum TypeWagon{
    Locomative = 1,
    NotSure = 3,
    Wagon = 0,
    NonStandard =2
};



class ConcreteMediator : public Mediator {
private:
    Camera* camera;
    Sensor* sensor;
    Circuit* circuit;
    Laser* laser;
    double TimeLastSend{0.0};
    int AccountAxle{0};
    int LastIndexSendDso{0};

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
        else
            endTime += 5;
        return endTime;
    }


    void PrintComponent(Data& data, int i,std::map<std::string, UskrData>& numberReturn,std::vector<UskrData>& coupleCurrent, std::vector<UskrData>& markCurrent){
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

    }

    void makeWagon(){
        Data& data = sensor->getData();
       // int QuantityCouple = data.ListIntervals.size();
        std::cout<<"------------------NUmmesssage: "<<data.NumMessage<<"--------------------------\n";


        double endTime = 0.0;
        bool flagRun = false;

        int CheckOrderDsoCouple = 0;
        for (int i=0; i<data.ListIntervals.size();i++){

            double StartData = stringToUnixTime(data.ListIntervals[i].StartDate);
            double EndDate = stringToUnixTime(data.ListIntervals[i].EndDate);
            double MinTime = TimeLastSend;
            double endTimeNext = getTimeCouple(data.ListIntervals,i,QuantityCouple);
            TimeLastSend =  endTimeNext;

            std::map<std::string, UskrData> numberReturn = camera->getNumber(MinTime, endTimeNext);
            std::vector<UskrData> coupleCurrent = camera->getCouple(EndData, endTimeNext);
            std::vector<UskrData> markCurrent = camera->getMark(StartData, endTimeNext);

            PrintComponent(data, i, numberReturn, coupleCurrent,  markCurrent);

            bool  EmptyAxle = (data.ListIntervals[i].Reliability==Reliability.Empty && data.ListIntervals[i].CountAxis>5);
            if (data.ListIntervals[i].Reliability!=Reliability.Empty  || EmptyAxle){
                if (data.ListIntervals[i].IsLastVagon || endTime==0.0)
                    if (endTime==0.0)
                        endTime = EndDate> endTime? EndDate : endTime;
                    else
                        endTime = EndDate;

                int sumAxleNumbers = std::accumulate(numberReturn.begin(), numberReturn.end(), 0,
                [](int accumulator, const std::pair<std::string, UskrData>& pair) {
                    return accumulator + pair.second.axle;});

                bool IsNumber = (data.ListIntervals[i].CountAxis==sumAxleNumbers);
                bool IsCouple = (coupleCurrent.size()>0 || data.ListIntervals[i].Reliability!=Reliability.NotSure);
                bool IsSure = data.ListIntervals[i].Reliability!=Reliability.Sure
                if (IsSure  || (IsCouple && IsNumber))
                    if  ( data.ListIntervals[i].CountAxis>0 || IsSure)
                        if (LastIndexSendDso >= data.ListIntervals[i].MinIndexAxle)
                           flagRun = false;
                    int RestAxle =  AccountAxle-data.ListIntervals[i].CountAxis;
                    bool CheckRestAxle = (RestAxle > 0 || endTime > 0.0);
                    if  data.ListIntervals[i].CountAxis > 0 && CheckOrderDsoCouple == i && flagRun && CheckRestAxle:
                        CheckOrderDsoCouple++;
                        data.ListIntervals[i].StatusWagon = true;
                        AccountAxle = RestAxle;
                        data.ListIntervals[i].StatusRecognition = coupleCurrent.size() + numberReturn.size();

                   else if (data.ListIntervals[i].CountAxis==0)
                         CheckOrderDsoCouple++;
                else if (data.ListIntervals[i].CountAxis==0 && IsSequenceViolation){
                    IsSequenceViolation = false;
                    CheckOrderDsoCouple++;
                }
            }
        }
    }

    void makeGraf(){


    }

    void notify(const std::string& sender, const std::string& event) override {
        TimeLastSend=getTime();
        if (event == "CameraDetected") {
            //std::cout << "Mediator reacts to motion detection." << std::endl;
        } else if (event == "SensorWork") {
              makeWagon();
              int numNeurons = 1; // Количество нейронов
              Graph graph(numNeurons);
              graph.addEdge(0, 4, 101);
              vector<int> predecessors;
              vector<float> distances = graph.bellmanFord(startNeuron, predecessors);
               if (!distances.empty()) {
                cout << "Расстояние от нейрона " << startNeuron << " до нейрона " << endNeuron << ": ";
                if (distances[endNeuron] == numeric_limits<float>::max()) {
                    cout << "Бесконечность" << endl;
                } else {
                    cout << distances[endNeuron] << endl;
                    printPath(startNeuron, endNeuron, predecessors); // Выводим путь
                }
                } else if (event == "CircuitActivated") {
                    // std::cout << "Mediator reacts to circuit activation." << std::endl;
        } else if (event == "LaserFired") {
            //std::cout << "Mediator reacts to laser firing." << std::endl;
        }
    }







};

#endif // CONCRETE_MEDIATOR_H



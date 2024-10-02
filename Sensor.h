#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

// Tag structure
struct Tag {
    int id;
    std::vector<long long> values;
};

// RealTimeCamInfo structure
struct RealTimeCamInfo {
    int Axle;
    int Type;
    std::string Number;
    int Quantity;
    std::string RealTime;
    Tag Time_mean;
    Tag Time_midle;
};

// Event structure
struct Event {
    std::string Time;
    int NumAxis;
    int Direction;
};

// Axis structure
struct Axis {
    int NumAxis;
    std::string LastUpdate;
    std::vector<Event> ListEventsDso;
    int DistanceAxisLeft;
    int DistanceAxisRight;
};

// StateScepka structure
struct StateScepka {
    int Send;
    int Status;
    int NumAxis;
    int NumMessage;
};

// Interval structure
struct Interval {
    int Dir;
    bool IsFull;
    std::string EndDate;
    std::vector<Axis> AxisList;
    int CountAxis;
    std::string StartDate;
    int Reliability;
    StateScepka stateScepka;
    std::vector<Event> AxisListReal;
    int TypeVagonByDso;
    RealTimeCamInfo realTimeCamInfo;
    int TypeReliability;
};

// Main data structure
struct Data {
    int NumMessage;
    std::vector<int> ModelVagons;
    std::vector<Interval> ListIntervals;
    std::vector<int> OutputStructs;
};



class Mediator; // Предварительное объявление

class Sensor {
private:
    Mediator* mediator;
    std::string name;
    Data data;
public:
    Sensor(Mediator* med, const std::string& n) : mediator(med), name(n) {}
    void update(const nlohmann::json& j);
    void dsoRun();
    void print();
    Data& getData();

};

#endif // SENSOR_H

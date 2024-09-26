#ifndef ModelDso_H
#define ModelDso_H

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


class Sensor {
public:
    static Sensor& getInstance() {
        static Sensor instance;
        return instance;
    }

    void updateJson(const nlohmann::json& json);
    void print();
private:
    Sensor() {} // private constructor
    Sensor(const Sensor&) = delete; // delete copy constructor
    Sensor& operator=(const Sensor&) = delete; // delete assignment operator
    Data data;

};


/*

class DataManager {
private:
    Data data;
    static DataManager* instance;
    static std::mutex mtx;

    // Private constructor
    DataManager() {}

public:
    // Get the singleton instance
    static DataManager* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance) {
            instance = new DataManager();
        }
        return instance;
    }

    // Update data from JSON
    void updateData(const std::string& jsonData);

    // Access data
    const Data& getData() const {
        return data;
    }
};*/
#endif
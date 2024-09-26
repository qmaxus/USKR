#ifndef ModelDso_H
#define ModelDso_H

#include <vector>
#include <mutex>
#include <iostream>
#include <nlohmann/json.hpp>

struct Tag;
struct RealTimeCamInfo;
struct Event;
struct Axis;
struct StateScepka ;
struct Interval;
//struct Data;
struct Data;

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
};
#endif
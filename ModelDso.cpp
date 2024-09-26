#include "ModelDso.h"
//#include <string>
#include <vector>
#include <mutex>
//#include <iostream>
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


DataManager::updateData(const std::string& jsonData) {
        auto j = json::parse(jsonData);

        data.NumMessage = j["NumMessage"];
        data.ModelVagons = j["ModelVagons"].get<std::vector<int>>();

        for (const auto& interval : j["ListIntervals"]) {
            Interval newInterval;
            newInterval.Dir = interval["Dir"];
            newInterval.IsFull = interval["IsFull"];
            newInterval.EndDate = interval["EndDate"];
            newInterval.CountAxis = interval["CountAxis"];
            newInterval.StartDate = interval["StartDate"];
            newInterval.Reliability = interval["Reliability"];
            newInterval.TypeVagonByDso = interval["TypeVagonByDso"];
            newInterval.TypeReliability = interval["TypeReliability"];
            if (interval.contains("StateScepka") && !interval.at("StateScepka").is_null()) {
                newInterval.stateScepka.Send = interval["StateScepka"]["Send"];
                newInterval.stateScepka.Status = interval["StateScepka"]["Status"];
                newInterval.stateScepka.NumAxis = interval["StateScepka"]["NumAxis"];
                newInterval.stateScepka.NumMessage = interval["StateScepka"]["NumMessage"];
            }
            for (const auto& axis : interval["AxisList"]) {
                Axis newAxis;
                newAxis.NumAxis = axis["NumAxis"];
                newAxis.LastUpdate = axis["LastUpdate"];

                for (const auto& event : axis["ListEventsDso"]) {
                    Event newEvent;
                    newEvent.Time = event["Time"];
                    newEvent.NumAxis = event["NumAxis"];
                    newEvent.Direction = event["Direction"];
                    newAxis.ListEventsDso.push_back(newEvent);
                }

                newAxis.DistanceAxisLeft = axis["DistanceAxisLeft"]["TypeDistanceAxis"];
                newAxis.DistanceAxisRight = axis["DistanceAxisRight"]["TypeDistanceAxis"];

                newInterval.AxisList.push_back(newAxis);
            }

            for (const auto& realAxis : interval["AxisListReal"]) {
                Event newRealAxis;
                newRealAxis.Time = realAxis["Time"];
                newRealAxis.NumAxis = realAxis["NumAxis"];
                newRealAxis.Direction = realAxis["Direction"];
                newInterval.AxisListReal.push_back(newRealAxis);
            }

            newInterval.realTimeCamInfo.Axle = interval["RealTimeCamInfo"]["Axle"];
            newInterval.realTimeCamInfo.Type = interval["RealTimeCamInfo"]["Type"];
            newInterval.realTimeCamInfo.Number = interval["RealTimeCamInfo"]["Number"];
            newInterval.realTimeCamInfo.Quantity = interval["RealTimeCamInfo"]["Quantity"];
            newInterval.realTimeCamInfo.RealTime = interval["RealTimeCamInfo"]["RealTime"];
            newInterval.realTimeCamInfo.Time_mean.id = interval["RealTimeCamInfo"]["Time_mean"]["id"];
            newInterval.realTimeCamInfo.Time_mean.values = interval["RealTimeCamInfo"]["Time_mean"]["values"].get<std::vector<long long>>();
            newInterval.realTimeCamInfo.Time_midle.id = interval["RealTimeCamInfo"]["Time_midle"]["id"];
            newInterval.realTimeCamInfo.Time_midle.values = interval["RealTimeCamInfo"]["Time_midle"]["values"].get<std::vector<long long>>();

            data.ListIntervals.push_back(newInterval);
        }

        data.OutputStructs = j["OutputStructs"].get<std::vector<int>>();
    }


/*

int main() {

    DataManager* manager = DataManager::getInstance();
    std::string jsonData = R"({
        "NumMessage": 64098,
        "ModelVagons": [],
        "ListIntervals": [{
            "Dir": 1,
            "IsFull": true,
            "EndDate": "2024-09-05T15:19:01.606",
            "AxisList": [{
                "NumAxis": 30867,
                "LastUpdate": "2024-09-05T15:19:01.606",
                "ListEventsDso": [{
                    "Time": "2024-09-05T15:18:59.982",
                    "NumAxis": 101547,
                    "Direction": 1
                }, {
                    "Time": "2024-09-05T15:19:01.606",
                    "NumAxis": 101631,
                    "Direction": 1
                }],
                "DistanceAxisLeft": {"TypeDistanceAxis": 2},
                "DistanceAxisRight": {"TypeDistanceAxis": 0}
            }],
            "CountAxis": 4,
            "StartDate": "2024-09-05T15:18:55.118",
            "Reliability": 0,
            "StateScepka": {
                "Send": 1,
                "Status": 2,
                "NumAxis": 30867,
                "NumMessage": 64092
            },
            "AxisListReal": [{
                "Time": "2024-09-05T15:19:01.606",
                "NumAxis": 101631,
                "Direction": 1
            }],
            "TypeVagonByDso": 0,
            "RealTimeCamInfo": {
                "Axle": 4,
                "Type": 0,
                "Number": "94485174",
                "Quantity": 100,
                "RealTime": "2024-09-05T15:18:59.389+03:00",
                "Time_mean": {"id": 4, "values": [-7, 17255387393893929]},
                "Time_midle": {"id": 4, "values": [-3, 1725538738441]}
            },
            "TypeReliability": 0
        }],
        "OutputStructs": []
    })";
     std::string jsonData1 = R"({
        "NumMessage": 64099,
        "ModelVagons": [],
        "ListIntervals": [{
            "Dir": 1,
            "IsFull": true,
            "EndDate": "2024-09-05T15:19:01.606",
            "AxisList": [{
                "NumAxis": 30867,
                "LastUpdate": "2024-09-05T15:19:01.606",
                "ListEventsDso": [{
                    "Time": "2024-09-05T15:18:59.982",
                    "NumAxis": 101547,
                    "Direction": 1
                }, {
                    "Time": "2024-09-05T15:19:01.606",
                    "NumAxis": 101631,
                    "Direction": 1
                }],
                "DistanceAxisLeft": {"TypeDistanceAxis": 2},
                "DistanceAxisRight": {"TypeDistanceAxis": 0}
            }],
            "CountAxis": 4,
            "StartDate": "2024-09-05T15:18:55.118",
            "Reliability": 0,
            "StateScepka": {
                "Send": 1,
                "Status": 2,
                "NumAxis": 30867,
                "NumMessage": 64092
            },
            "AxisListReal": [{
                "Time": "2024-09-05T15:19:01.606",
                "NumAxis": 101631,
                "Direction": 1
            }],
            "TypeVagonByDso": 0,
            "RealTimeCamInfo": {
                "Axle": 4,
                "Type": 0,
                "Number": "94485174",
                "Quantity": 100,
                "RealTime": "2024-09-05T15:18:59.389+03:00",
                "Time_mean": {"id": 4, "values": [-7, 17255387393893929]},
                "Time_midle": {"id": 4, "values": [-3, 1725538738441]}
            },
            "TypeReliability": 0
        }],
        "OutputStructs": []
    })";
   // manager->updateData(jsonData);
   // const Data& data = manager->getData();
    //std::cout << "NumMessage: " << data.NumMessage << std::endl;

    manager->updateData(jsonData);
    std::cout << "Number of messages: " << &manager << std::endl;

    manager->updateData(jsonData1);
    std::cout << "Number of messages: " << &manager << std::endl;

    return 0;
}*/
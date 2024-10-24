#include "Sensor.h"
#include "../Mediator.h"
#include <map>
#include <algorithm>

void Sensor::dsoRun() {
    mediator->notify(name, "SensorWork");
}


Data& Sensor::getData(){
    return data;
 }
void Sensor::print() {
    std::cout << name << " --DSO MSG." <<data.NumMessage << std::endl;
}


enum Direction{
    Left = 1,
    Right = 0,
    Unknown = 2
};





int maxElement(std::map<int, int>& map, int defaultValue){
return map.empty()? defaultValue : std::max_element(map.begin(), map.end(),
[](const auto& a, const auto& b){return a.second < b.second;})->second;
}

void Sensor::update(const nlohmann::json& j) {
    data.NumMessage = j["c"]["NumMessage"];
    data.ModelVagons = j["c"]["ModelVagons"].get<std::vector<int>>();
    data.ListIntervals.clear();
    for (const auto& interval : j["c"]["ListIntervals"]) {
        Interval newInterval;
        newInterval.Dir = interval["Dir"];
        newInterval.IsLastVagon = interval["IsLastVagon"];
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
        std::map<int,int > maxAccountDir;
        int MaxIndexAxle;
        int MinIndexAxle;
        for (const auto& axis : interval["AxisList"]) {
            Axis newAxis;
            newAxis.NumAxis = axis["NumAxis"];
            if(newAxis.NumAxis < MinIndexAxle)
                MinIndexAxle = newAxis.NumAxis;
            if(newAxis.NumAxis > MaxIndexAxle)
                MaxIndexAxle = newAxis.NumAxis;

            newAxis.LastUpdate = axis["LastUpdate"];
            for (const auto& event : axis["ListEventsDso"]) {
                Event newEvent;
                newEvent.Time = event["Time"];
                newEvent.NumAxis = event["NumAxis"];
                newEvent.Direction = event["Direction"];
                if (newEvent.Direction!=Unknown){
                    if (maxAccountDir.count(newEvent.Direction) > 0)
                         maxAccountDir[newEvent.Direction]++;
                    else
                        maxAccountDir[newEvent.Direction] =1;
                }
                newAxis.ListEventsDso.push_back(newEvent);
            }
            newAxis.DistanceAxisLeft = axis["DistanceAxisLeft"]["TypeDistanceAxis"];
            newAxis.DistanceAxisRight = axis["DistanceAxisRight"]["TypeDistanceAxis"];
            newInterval.AxisList.push_back(newAxis);
        }
        if (newInterval.Dir == Unknown)
            newInterval.Dir = maxElement(maxAccountDir, Unknown);

        newInterval.MaxIndexAxle = MaxIndexAxle;
        newInterval.MinIndexAxle = MinIndexAxle;

        for (const auto& realAxis : interval["AxisListReal"]) {
            Event newRealAxis;
            newRealAxis.Time = realAxis["Time"];
            newRealAxis.NumAxis = realAxis["NumAxis"];
            newRealAxis.Direction = realAxis["Direction"];
            newInterval.AxisListReal.push_back(newRealAxis);
        }
        if (typeid(interval["RealTimeCamInfo"]) == typeid(std::map<std::string, int>)){
            newInterval.realTimeCamInfo.Axle = interval["RealTimeCamInfo"]["Axle"];
            newInterval.realTimeCamInfo.Type = interval["RealTimeCamInfo"]["Type"];
            newInterval.realTimeCamInfo.Number = interval["RealTimeCamInfo"]["Number"];
            newInterval.realTimeCamInfo.Quantity = interval["RealTimeCamInfo"]["Quantity"];
            newInterval.realTimeCamInfo.RealTime = interval["RealTimeCamInfo"]["RealTime"];
            newInterval.realTimeCamInfo.Time_mean.id = interval["RealTimeCamInfo"]["Time_mean"]["id"];
            newInterval.realTimeCamInfo.Time_mean.values = interval["RealTimeCamInfo"]["Time_mean"]["values"].get<std::vector<long long>>();
            newInterval.realTimeCamInfo.Time_midle.id = interval["RealTimeCamInfo"]["Time_midle"]["id"];
            newInterval.realTimeCamInfo.Time_midle.values = interval["RealTimeCamInfo"]["Time_midle"]["values"].get<std::vector<long long>>();
        }

        data.ListIntervals.push_back(newInterval);
    }
    data.OutputStructs = j["c"]["OutputStructs"].get<std::vector<int>>();
    dsoRun();
}


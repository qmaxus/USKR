#include "Sensor.h"
#include "Mediator.h"

void Sensor::triggerAlarm() {
    std::cout << name << " DSO MSG." <<data.NumMessage << std::endl;
    mediator->notify(name, "AlarmTriggered");
}

void Sensor::update(const nlohmann::json& j) {
    std::cout << name << " update dso" <<j["c"]["NumMessage"]<< std::endl;
    data.NumMessage = j["c"]["NumMessage"];
    data.ModelVagons = j["c"]["ModelVagons"].get<std::vector<int>>();

    for (const auto& interval : j["c"]["ListIntervals"]) {

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
        data.ListIntervals.clear();
        data.ListIntervals.push_back(newInterval);
    }

    data.OutputStructs = j["c"]["OutputStructs"].get<std::vector<int>>();
    std::cout << name << " ------stop update dso" << std::endl;
}


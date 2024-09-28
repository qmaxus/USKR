#include "Camera.h"
#include "Mediator.h"

void Camera::update(const nlohmann::json& json) {
    std::cout<<"Number"<<" \n";
    //Number update
    for (const auto& value : json["info"]["number"]) {

         std::cout<<"INIT"<<value["info"]<<number.count(value["info"])<<" \n";
         if (number.count(value["info"])>0)
         {
             std::cout<<"updateNumber"<<" \n";
             updateDataRecognition(value,value["info"], json["time"], json["id"], number[value["info"]]);
         }
         else {
            std::cout<<"INIT"<<" \n";
            UskrData data = initDataRecognition( value,value["info"], json["time"], json["id"]);
            number[value["info"]]=data;
            std::cout<<"addd "<<value["info"]<<number.count(value["info"])<<" \n";

         }
    }
     std::cout<<"Couple"<<" \n";
    //Couple update
    for (const auto& value : json["info"]["coupl"]) {

            bool updateFlag{false};
            if (!couple.empty()){

                UskrData& last_couple = couple.back();
                double limitSecond = 2.2;// add config
                bool limitMax = std::abs(last_couple.timeMax - static_cast<double>(json["time"]) )< limitSecond;
                bool limitMin = std::abs(last_couple.timeMax - static_cast<double>(json["time"]) )< limitSecond;
                if ( last_couple.timeMin<= static_cast<double>(json["time"])<=last_couple.timeMax  || limitMax  ||  limitMin )
                {
                    std::cout<<"updateCouple"<<" \n";
                    updateDataRecognition(value,"couple", json["time"], json["id"], last_couple);
                    updateFlag =true;
                }
            }
            if (!updateFlag) {
                UskrData data=initDataRecognition(value,"couple", json["time"], json["id"]);
                couple.push_back(data);
            }
    }
    //Mark update
    std::cout<<"Mark"<<" \n";
    for (const auto& value : json["info"]["mark"]) {
        std::cout<<value<<" \n";
    }
    mediator->notify(name, "MotionDetected");
}

double calculateMean(const std::vector<double>& numbers) {
    double sum = 0.0;
    for (double num : numbers) {
        sum += num;
    }
    return sum / numbers.size();
}

Coordinate writePosition(const nlohmann::json& json,std::string nameObject){
    Coordinate coordinate;
    coordinate.xMin = static_cast<int>(json["c"][0]);
    coordinate.yMin = static_cast<int>(json["c"][1]);
    if (nameObject=="couple" || nameObject=="mark"){
        coordinate.xMax = static_cast<int>(json["c"][2]);
        coordinate.xMax = static_cast<int>(json["c"][3]);
    }
    else{
        coordinate.xMax = static_cast<int>(json["c"][0]) + static_cast<int>(json["c"][2]);
        coordinate.xMax = static_cast<int>(json["c"][1]) + static_cast<int>(json["c"][3]);
    }
    return coordinate;
}


void Camera::updateDataRecognition(const nlohmann::json& json,std::string nameObject, double timeFrame, int idCam, UskrData& data)
{

    if (data.cameraId.count(idCam)>0)
    {
        data.cameraId[idCam]++;
        std::cout<<"+++"<<" \n";
    }
    else
    {
        std::cout<<"==1"<<" \n";
        data.cameraId[idCam]=1;
    }
    Frame frame;
    frame.time = timeFrame;
    Coordinate coordinate = writePosition(json, nameObject);
    if ( json["trec"].is_string())
        frame.typeRecognition = json["trec"];
    else
        frame.typeRecognition = static_cast<std::string>(json["trec"][0]);

    frame.probabilityFrame = json["p"];
    frame.coordinate = coordinate;
    data.frames.push_back(frame);
    if (timeFrame<data.timeMin)
        data.timeMin = timeFrame;
    if (timeFrame>data.timeMax)
        data.timeMax = timeFrame;

    data.probability = (static_cast<double>(json["p"]) + data.probability)/2.0;
    double middleCouple = (coordinate.xMin + coordinate.xMax)/2.0;
    std::vector<double> middleDso = config["client"][std::to_string(data.cameraMiddle)]["limit_dso"];
    double currentDistance = std::abs(calculateMean(middleDso) - data.middlePosition);
    double newDistance = std::abs(calculateMean(config["client"][std::to_string(idCam)]["limit_dso"]) - middleCouple);
    if (currentDistance>newDistance){
        data.time = timeFrame;
        data.cameraMiddle = idCam;
        data.middlePosition = middleCouple;
    }




}

UskrData Camera::initDataRecognition(const nlohmann::json& json,std::string nameObject, double timeFrame, int idCam){

    UskrData data;
    data.info = nameObject;
    data.time = timeFrame;
    data.probability=json["p"];
    data.cameraId[idCam] = 1;
    Frame frame;
    frame.time = timeFrame;
    Coordinate coordinate = writePosition(json, nameObject);
    if ( json["trec"].is_string())
        frame.typeRecognition = json["trec"];
    else
        frame.typeRecognition = static_cast<std::string>(json["trec"][0]);
    frame.probabilityFrame = json["p"];
    frame.coordinate = coordinate;
    data.frames.push_back(frame);
    data.timeMin = timeFrame;
    data.timeMax = timeFrame;
    data.cameraMiddle = idCam;
    data.middlePosition = (coordinate.xMin+coordinate.xMax)/2.0;
    return data;
}


void Camera::deleted(double timeEnd){
    for (auto it = number.begin(); it != number.end();) {
        if (it->second.time < timeEnd) {
            it = number.erase(it);
        } else {
            ++it;
        }
    }

}

void Camera::print() {
    for (const auto& item : number) {
        auto key = item.first;
        auto data = item.second;

        std::cout << "Info: " << data.info << std::endl;
        std::cout << "Time: " << data.time << std::endl;
        std::cout << "Probability: " << data.probability << std::endl;
        std::cout << "Camera ID: ";
        for (const auto& pair : data.cameraId) {
            std::cout << pair.first << " -> " << pair.second << ", ";
        }
        std::cout << std::endl;
        std::cout << "timeMin: " << data.timeMin << std::endl;
        std::cout << "timeMax: " << data.timeMax << std::endl;
        std::cout << "Axle: " << data.axle << std::endl;
        std::cout << "typeWagon: " << data.typeWagon << std::endl;
        std::cout << "MiddlePosition: " << data.middlePosition << std::endl;
        std::cout << std::endl;
    }
}

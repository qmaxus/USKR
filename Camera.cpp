#include "Camera.h"
#include "Mediator.h"

void Camera::update(const nlohmann::json& json) {

    for (const auto& value : json["info"]["number"]) {

         if (number.count(value["info"])>0){
             ++number[value["info"]].quantity;
            // number[value["info"]].time.push_back(json["time"]);
             if (number[value["info"]].cameraId.count(json["id"])>0)
                ++number[value["info"]].cameraId[json["id"]];
             else
                 number[value["info"]].cameraId[json["id"]]=1;
             number[value["info"]].change = true;
             number[value["info"]].timeMean = (static_cast<double>(json["time"])+number[value["info"]].timeMean) / 2.0;
         }
         else {
            UskrData data;
            data.number = value["info"];
            data.quantity = 1;
            data.time.push_back(json["time"]);
            data.cameraId[json["id"]] = 1;
            //NumberInfo axleType = qaxl_number(dataAxleNum,value["info"]);
           // data.type = axleType.typeWagon;
            //data.axle = axleType.axle;
            data.change = true;
            data.timeMean = json["time"];
            number[value["info"]]=data;
         }
    }

    for (const auto& value : json["info"]["coupl"]) {

        std::cout<<value<<" \n";
    }
    for (const auto& value : json["info"]["mark"]) {
        std::cout<<value<<" \n";
    }
    mediator->notify(name, "MotionDetected");
}



void Camera::print() {
    for (const auto& item : number) {
        auto key = item.first;
        auto data = item.second;

        std::cout << "Number: " << data.number << std::endl;
        std::cout << "Quantity: " << data.quantity << std::endl;
        std::cout << "Time: ";
        for (double t : data.time) {
            std::cout << t << " ";
        }
        std::cout << std::endl;
        std::cout << "Time Middle: " << data.time_middle << std::endl;
        std::cout << "Camera ID: ";
        for (const auto& pair : data.cameraId) {
            std::cout << pair.first << " -> " << pair.second << ", ";
        }
        std::cout << std::endl;
        std::cout << "Type: " << data.type << std::endl;
        std::cout << "Axle: " << data.axle << std::endl;
        std::cout << "Change: " << (data.change ? "true" : "false") << std::endl;
        std::cout << "Time Mean: " << data.timeMean << std::endl;
        std::cout << std::endl;
    }
}

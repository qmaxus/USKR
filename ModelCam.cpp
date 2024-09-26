#include "ModelCam.h"

double getTime(){
    auto now = std::chrono::high_resolution_clock::now();
    auto unixTime = static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()) +
                    static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() % 1000000) / 1000000.0;
    return unixTime;
 }


void Camera::print(){
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

void Camera::updateJson(const nlohmann::json& json, std::vector<std::vector<int>>& dataAxleNum) {

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
            NumberInfo axleType = qaxl_number(dataAxleNum,value["info"]);
            data.type = axleType.typeWagon;
            data.axle = axleType.axle;
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

}

void Camera::takePicture() {
    // implementation
}




std::vector<std::vector<int>> load_file(const std::string& filename, int skiprows) {
    std::vector<std::vector<int>> data;
    std::ifstream file(filename);
    std::string line;
    for (int i = 0; i < skiprows; i++) {
        std::getline(file, line);
    }
    // Read the data
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int val;
        while (iss >> val) {
            row.push_back(val);
        }
        data.push_back(row);
    }

    return data;
}


NumberInfo qaxl_number(const std::vector<std::vector<int>>& data, const std::string& num) {

    NumberInfo AxleType;
    for (const auto& row : data) {
        if (row[0] <= std::stoi(num.substr(0, 6)) &&
            row[1] >= std::stoi(num.substr(0, 6)) &&
            row[2] <= std::stoi(num.substr(6,1)) &&
            row[3] >= std::stoi(num.substr(6,1))) {
            // if (list_index_vagon_articulated.count(row[4])) {

            AxleType.typeWagon = 0;  //list_index_vagon_articulated.at(row[4]);
            //}
            AxleType.axle = row[9];
            break;
        }
    }
    return AxleType;

}

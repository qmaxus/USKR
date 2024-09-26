#include "ModelCam.h"

double getTime(){
    auto now = std::chrono::high_resolution_clock::now();
    auto unixTime = static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()) +
                    static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() % 1000000) / 1000000.0;
    return unixTime;
 }


void Cam::print(){
    for (const auto& pair : number) {
        std::cout << "Key: " << pair.second.quantity << ", Value: ";
    }
}

void Cam::updateJson(const nlohmann::json& json, std::vector<std::vector<int>>& dataAxleNum) {
   /*
   // uskrData.clear();
    for (const auto& uskr : json["number_uskr"]) {
        UskrData data;
        data.number = uskr["number"];
        data.quantity = uskr["quantity"];
        for (const auto& lastTime : uskr["time"])
            data.time.push_back(lastTime);
        for (const auto& item : uskr["cam"].items())
        {
            auto key = item.key();
            auto value = item.value();
            data.cameraId[key]= value;
       }
        //data.time_middle = uskr["time_midle"];
        data.type = uskr["type"];
        data.axle = uskr["axle"];
        data.change = uskr["change"];
        data.time_mean = uskr["time_mean"];
        uskrData.push_back(data);
    }
    */

    for (const auto& value : json["info"]["number"]) {

         if (number.count(value["info"])>0){

             ++number[value["info"]].quantity;
             //for (const auto& lastTime : value["time"])
               // data.time.push_back(lastTime);
           //  for (const auto& item :value["cam"].items())
            // {
              //  auto key = item.key();
               // auto value = item.value();
               // data.cameraId[key]= value;
             //}
            // data.type = value["type"];
             //data.axle = value["axle"];
             //data.change = value["change"];
           //  data.time_mean = value["time_mean"];
            // number[value["info"]]=data;

         }
         else {

            UskrData data;
            data.quantity = 1;
            data.time.push_back(json["time"]);
            data.cameraId[json["id"]] = 1;

            NumberInfo axleType = qaxl_number(dataAxleNum,value["info"]);
            data.type = axleType.typeWagon;
            data.axle = axleType.axle;
            //data.change = value["change"];
            data.time_mean = json["time"];
            number[value["info"]]=data;
         }
    }

    for (const auto& value : json["info"]["coupl"]) {
        std::cout<<value<<" \n";
    }
    for (const auto& value : json["info"]["mark"]) {
        std::cout<<value<<" \n";
    }
     std::cout<<"value"<<" \n";
}

void Cam::takePicture() {
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

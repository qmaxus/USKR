#ifndef MODELCAM_H
#define MODELCAM_H
#include <iostream>
#include <nlohmann/json.hpp> // for JSON support
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <sstream>

struct NumberInfo{
  int axle{0};
  int  typeWagon{0};
};

NumberInfo qaxl_number(const std::vector<std::vector<int>>& data, const std::string& num);
std::vector<std::vector<int>> load_file(const std::string& filename, int skiprows = 1);
double getTime();

struct UskrData {
    std::string number;
    int quantity;
    std::vector<double> time;
    double time_middle;
    std::map<int, int> cameraId;
    int type;
    int axle;
    bool change;
    double timeMean;
};


class Camera {
public:
    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }
    void takePicture();
    void updateJson(const nlohmann::json& json, std::vector<std::vector<int>>& dataAxleNum);
    void print();
private:
    Camera() {} // private constructor
    Camera(const Camera&) = delete; // delete copy constructor
    Camera& operator=(const Camera&) = delete; // delete assignment operator
    std::map<std::string, UskrData> number;

};


#endif  // MODELCAM_H

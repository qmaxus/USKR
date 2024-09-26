#ifndef MODELCAM_H_
#define MODELCAM_H_
#include <iostream>
#include <nlohmann/json.hpp> // for JSON support
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <sstream>
/*
std::map<int, int> list_index_vagon_articulated = {
    {203, 2}, {604, 2}, {797, 2}, {799, 2}, {957, 2}, {958, 2}, {5984, 2},
    {551, 1}, {552, 1}, {553, 1}, {554, 1}, {555, 1}, {556, 1}, {557, 1},
    {558, 1}, {559, 1}, {560, 1}, {561, 1}, {562, 1}, {563, 1}, {564, 1},
    {565, 1}, {566, 1}, {567, 1}, {568, 1}, {569, 1}, {570, 1}, {571, 1},
    {572, 1}, {573, 1}, {574, 1}, {575, 1}, {576, 1}, {577, 1}, {578, 1},
    {579, 1}, {580, 1}, {581, 1}, {582, 1}, {583, 1}, {584, 1}, {585, 1},
    {586, 1}
};*/

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
    double time_mean;
};


class Cam {
public:
    static Cam& getInstance() {
        static Cam instance;
        return instance;
    }
    void takePicture();
    void updateJson(const nlohmann::json& json, std::vector<std::vector<int>>& dataAxleNum);
    void print();
private:
    Cam() {} // private constructor
    Cam(const Cam&) = delete; // delete copy constructor
    Cam& operator=(const Cam&) = delete; // delete assignment operator
    //std::vector<UskrData> uskrData;
    std::map<std::string, UskrData> number;

};


#endif  // MODELCAM_H

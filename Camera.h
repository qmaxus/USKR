#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>


class Mediator; // Предварительное объявление


struct UskrNumber {
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

struct Coordinate{
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

struct Frame{
	double time;
	int id;
	Coordinate coordinate;
	std::string typeRecognition;
	double probabilityFrame;
};


struct UskrData{
    std::string info;
	double time;
	double probability;
	std::map<int, int> cameraId;
	std::vector<Frame> frames;
	double timeMin;
	double timeMax;
	int cameraMiddle;
	int axle;
	int typeWagon;
	double middlePosition;
};


struct UskrCouple{
	double time;
	double probability;
	std::vector<int> camera;
	std::vector<Frame> frames;
	double timeMin;
	double timeMax;
	double limit;
	int cameraMiddle;
	double middlePosition;
};


struct UskrMark {
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
private:
    Mediator* mediator;
    std::string name;
    std::map<std::string, UskrData> number;
    std::vector<UskrData> couple;
    std::vector<UskrData> mark;
    const nlohmann::json& config;
public:
    Camera(Mediator* med, const std::string& n, const nlohmann::json& config_file) : mediator(med), name(n), config(config_file) {}
    void deleted(double timeEnd);
    UskrData initDataRecognition(const nlohmann::json& json,std::string nameObject, double timeFrame, int idCam);
    void updateDataRecognition(const nlohmann::json& json,std::string nameObject, double timeFrame, int idCam, UskrData& data);
    void update(const nlohmann::json& json);
    void print();
    UskrData getNumber(double timeStart, double timeEnd);
    UskrData getCouple(double timeStart, double timeEnd);
    UskrData getMark(double timeStart, double timeEnd);
};

#endif // CAMERA_H
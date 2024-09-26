#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>


class Mediator; // Предварительное объявление


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


struct UskrCouple{
	double time;
	double probability;
	std::vector<int> camera;
	std::vector<std::map<Frame>> frame;
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
public:
    Camera(Mediator* med, const std::string& n) : mediator(med), name(n) {}
    void deleted(double timeEnd);
    void update(const nlohmann::json& json);
    void print();
};

#endif // CAMERA_H
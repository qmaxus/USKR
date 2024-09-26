#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>

class Mediator; // Предварительное объявление

class Camera {
private:
    Mediator* mediator;
    std::string name;
public:
    Camera(Mediator* med, const std::string& n) : mediator(med), name(n) {}

    void detectMotion();
    void takePicture();
};

#endif // CAMERA_H
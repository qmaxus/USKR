#include "Camera.h"
#include "Mediator.h"

void Camera::detectMotion() {
    std::cout << name << " detected motion." << std::endl;
    mediator->notify(name, "MotionDetected");
}

void Camera::takePicture() {
    std::cout << name << " takes a picture." << std::endl;
}
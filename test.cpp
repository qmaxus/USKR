#include <iostream>
#include <string>

// Интерфейс посредника
class Mediator {
public:
    virtual void notify(const std::string& sender, const std::string& event) = 0;
};

// Класс камеры
class Camera {
private:
    Mediator* mediator;
    std::string name;
public:
    Camera(Mediator* med, const std::string& n) : mediator(med), name(n) {}

    void detectMotion() {
        std::cout << name << " detected motion." << std::endl;
        mediator->notify(name, "MotionDetected");
    }

    void takePicture() {
        std::cout << name << " takes a picture." << std::endl;
    }
};

// Класс датчика
class Sensor {
private:
    Mediator* mediator;
    std::string name;
public:
    Sensor(Mediator* med, const std::string& n) : mediator(med), name(n) {}

    void triggerAlarm() {
        std::cout << name << " triggered an alarm." << std::endl;
        mediator->notify(name, "AlarmTriggered");
    }
};

// Конкретный посредник (Одиночка)
class ConcreteMediator : public Mediator {
private:
    Camera* camera;
    Sensor* sensor;

    // Приватный конструктор для реализации одиночки
    ConcreteMediator() : camera(nullptr), sensor(nullptr) {}

public:
    // Метод для получения экземпляра одиночки
    static ConcreteMediator& getInstance() {
        static ConcreteMediator instance;
        return instance;
    }

    // Устанавливаем камеры и датчики
    void setComponents(Camera* cam, Sensor* sen) {
        camera = cam;
        sensor = sen;
    }

    void notify(const std::string& sender, const std::string& event) override {
        if (event == "MotionDetected") {
            std::cout << "Mediator reacts to motion detection." << std::endl;
            camera->takePicture();
            sensor->triggerAlarm();
        } else if (event == "AlarmTriggered") {
            std::cout << "Mediator reacts to alarm trigger." << std::endl;
            // Дополнительные действия при срабатывании тревоги
        }
    }
};

int main() {
    // Создаем объекты камеры и датчика
    Camera camera(&ConcreteMediator::getInstance(), "Camera1");
    Sensor sensor(&ConcreteMediator::getInstance(), "Sensor1");

    // Устанавливаем компоненты в посредник
    ConcreteMediator::getInstance().setComponents(&camera, &sensor);

    // Симуляция событий
    camera.detectMotion(); // Камера обнаруживает движение
    sensor.triggerAlarm();  // Датчик срабатывает

    return 0;
}
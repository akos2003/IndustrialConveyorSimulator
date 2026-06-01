#pragma once
#include <string>

class Sensor {
private:
    int id;
    bool active;
    std::string name;

public:
    Sensor(int sensorId, const std::string& sensorName);

    void trigger();
    void reset();

    bool isActive() const;
    int getId() const;
    std::string getName() const;
};
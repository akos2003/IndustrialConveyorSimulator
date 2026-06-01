#include "Sensor.h"

Sensor::Sensor(int sensorId, const std::string& sensorName) {
    id = sensorId;
    name = sensorName;
    active = false;
}

void Sensor::trigger() {
    active = true;
}

void Sensor::reset() {
    active = false;
}

bool Sensor::isActive() const {
    return active;
}

int Sensor::getId() const {
    return id;
}

std::string Sensor::getName() const {
    return name;
}
#include "Motor.h"
#include <iostream>

Motor::Motor() {
    running = false;
}

void Motor::start() {
    running = true;
}

void Motor::stop() {
    running = false;
}

bool Motor::isRunning() const {
    return running;
}
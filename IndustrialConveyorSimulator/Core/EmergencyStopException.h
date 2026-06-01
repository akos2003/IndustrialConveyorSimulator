#pragma once
#include <stdexcept>

class EmergencyStopException : public std::runtime_error {
public:
    EmergencyStopException() : std::runtime_error("CRITICAL: Emergency Stop Activated!") {}
};
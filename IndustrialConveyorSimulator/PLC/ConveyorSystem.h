#pragma once
#include <memory>
#include <vector>
#include "../Core/ConveyorState.h"
#include "../Core/EmergencyStopException.h"
#include "../Hardware/Motor.h"
#include "../Hardware/Sensor.h"
#include "../Logging/ILogger.h"

class ConveyorSystem {
private:
    ConveyorState currentState;
    std::unique_ptr<ILogger> logger;
    std::unique_ptr<Motor> mainMotor;
    std::vector<std::unique_ptr<Sensor>> sensors;

public:
    ConveyorSystem(std::unique_ptr<ILogger> externalLogger);

    void addSensor(std::unique_ptr<Sensor> sensor);
    void startProduction();
    void stopProduction();
    void triggerEmergencyStop();
    void update();
    void triggerSensor(int sensorId);
    void getSensorDetails();

    void runScenario(int scenarioId);
};
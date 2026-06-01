#include "ConveyorSystem.h"
#include <thread>
#include <chrono>

ConveyorSystem::ConveyorSystem(std::unique_ptr<ILogger> externalLogger) {
    logger = std::move(externalLogger);
    mainMotor = std::make_unique<Motor>();
    currentState = ConveyorState::Idle;

    logger->log("PLC System: Initialized. State: IDLE.");
}

void ConveyorSystem::addSensor(std::unique_ptr<Sensor> sensor) {
    logger->log("PLC System: New sensor connected -> " + sensor->getName());
    sensors.push_back(std::move(sensor));
}

void ConveyorSystem::startProduction() {
    if (currentState == ConveyorState::EmergencyStopped) {
        logger->log("PLC ERROR: Cannot start, emergency stop is active!");
        return;
    }

    if (!mainMotor->isRunning()) {
        mainMotor->start();
        currentState = ConveyorState::Running;
        logger->log("PLC System: Main motor started. State: RUNNING.");
    }
}

void ConveyorSystem::stopProduction() {
    if (mainMotor->isRunning()) {
        mainMotor->stop();
        currentState = ConveyorState::Idle;
        logger->log("PLC System: Main motor stopped. State: IDLE.");
    }
}

void ConveyorSystem::triggerEmergencyStop() {
    mainMotor->stop();
    currentState = ConveyorState::EmergencyStopped;

    logger->log("!!! EMERGENCY !!! Main motor stopped immediately.");

    throw EmergencyStopException();
}

void ConveyorSystem::update() {
    if (currentState != ConveyorState::Running) {
        return;
    }

    for (const auto& sensor : sensors) {
        if (sensor->isActive()) {
            logger->log("PLC System: Event on sensor (ID: " +
                std::to_string(sensor->getId()) + ") -> " + sensor->getName());
            sensor->reset();
        }
    }
}

void ConveyorSystem::triggerSensor(int sensorId) {
    for (const auto& sensor : sensors) {
        if (sensor->getId() == sensorId) {
            sensor->trigger();
            return;
        }
    }
    logger->log("PLC WARNING: Sensor ID " + std::to_string(sensorId) + " not found.");
}

void ConveyorSystem::getSensorDetails() {
    logger->log("PLC System: Connected sensors:");
    for (const auto& sensor : sensors) {
        logger->log("Id: " + std::to_string(sensor->getId()) +
            " - Name: " + sensor->getName() +
            " - State: " + (sensor->isActive() ? "Active" : "Inactive"));
    }
}

void ConveyorSystem::runScenario(int scenarioId) {
    if (scenarioId == 1) {
        logger->log("SCENARIO 1: Batch processing of 3 boxes initiated.");
        startProduction();

        for (int i = 1; i <= 3; ++i) {
            logger->log("SCENARIO 1: Box " + std::to_string(i) + " entering conveyor.");

            std::this_thread::sleep_for(std::chrono::seconds(1));
            triggerSensor(1);
            update();

            std::this_thread::sleep_for(std::chrono::seconds(2));
            triggerSensor(2);
            update();

            std::this_thread::sleep_for(std::chrono::seconds(1));
            triggerSensor(99);
            update();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        stopProduction();
        logger->log("SCENARIO 1: Batch completed successfully.");

    }
    else if (scenarioId == 2) {
        logger->log("SCENARIO 2: Jam simulation at packaging initiated.");
        startProduction();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        triggerSensor(1);
        update();

        std::this_thread::sleep_for(std::chrono::seconds(2));
        triggerSensor(2);
        update();

        std::this_thread::sleep_for(std::chrono::seconds(2));
        logger->log("SCENARIO 2: CRITICAL - Box stuck before packaging sensor!");

        std::this_thread::sleep_for(std::chrono::seconds(1));
        triggerEmergencyStop();

    }
    else {
        logger->log("PLC WARNING: Unknown scenario ID.");
    }
}
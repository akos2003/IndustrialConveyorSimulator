#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include "Logging/ConsoleLogger.h"
#include "PLC/ConveyorSystem.h"
#include "Hardware/Sensor.h"
#include "Core/EmergencyStopException.h"

std::mutex consoleMutex;

void backgroundUpdateLoop(ConveyorSystem& conveyor, bool& isRunning) {
    try {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::lock_guard<std::mutex> lock(consoleMutex);
            if (isRunning) {
                conveyor.update();
            }
        }
    }
    catch (const EmergencyStopException& e) {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cerr << "\n[Background Thread] Emergency Stop Exception Caught: " << e.what() << "\n";
        isRunning = false;
    }
}

int main() {
    bool isRunning = true;
    std::thread updateThread;

    try {
        std::unique_ptr<ILogger> logger = std::make_unique<ConsoleLogger>();
        ConveyorSystem conveyor(std::move(logger));

        conveyor.addSensor(std::make_unique<Sensor>(1, "EntrySensor"));
        conveyor.addSensor(std::make_unique<Sensor>(2, "ExitSensor"));
        conveyor.addSensor(std::make_unique<Sensor>(99, "PackagingSensor"));

        std::cout << "========================================\n";
        std::cout << "Commands: start, stop, s<id>, details, sc1, sc2, emg, exit\n";
        std::cout << "========================================\n";

        updateThread = std::thread(backgroundUpdateLoop, std::ref(conveyor), std::ref(isRunning));

        std::string command;

        while (isRunning) {
            {
                std::lock_guard<std::mutex> lock(consoleMutex);
                if (!isRunning) break;
                std::cout << "> ";
            }

            std::cin >> command;

            std::lock_guard<std::mutex> lock(consoleMutex);
            if (!isRunning) break;

            if (command == "start") {
                conveyor.startProduction();
            }
            else if (command == "stop") {
                conveyor.stopProduction();
            }
            else if (command == "details") {
                conveyor.getSensorDetails();
            }
            else if (command == "sc1") {
                conveyor.runScenario(1);
            }
            else if (command == "sc2") {
                conveyor.runScenario(2);
            }
            else if (command == "emg") {
                conveyor.triggerEmergencyStop();
                isRunning = false;
            }
            else if (command == "exit") {
                isRunning = false;
            }
            else if (command.length() > 1 && command[0] == 's') {
                try {
                    int sensorId = std::stoi(command.substr(1));
                    conveyor.triggerSensor(sensorId);
                }
                catch (...) {
                    std::cout << "Invalid sensor ID format.\n";
                }
            }
            else {
                std::cout << "Invalid command.\n";
            }
        }

    }
    catch (const EmergencyStopException& e) {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cerr << "\n[Main Thread] Emergency Stop Exception Caught: " << e.what() << "\n";
        isRunning = false;
    }

    if (updateThread.joinable()) {
        updateThread.join();
    }

    return 0;
}
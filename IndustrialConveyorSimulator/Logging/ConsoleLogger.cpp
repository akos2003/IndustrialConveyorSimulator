#define _CRT_SECURE_NO_WARNINGS
#include "ConsoleLogger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <windows.h>

void SetConsoleTextColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void ConsoleLogger::log(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    if (message.find("ERROR") != std::string::npos || message.find("EMERGENCY") != std::string::npos) {
        SetConsoleTextColor(4, 0);
    }
    else if (message.find("WARNING") != std::string::npos) {
        SetConsoleTextColor(14, 0);
    }
    else {
        SetConsoleTextColor(7, 0);
    }

    std::cout << "[" << std::put_time(std::localtime(&nowTime), "%H:%M:%S") << "] "
        << message << "\n";

    SetConsoleTextColor(7, 0);
}
#pragma once
#include "ILogger.h"
#include <string>

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override;
    void SetColor(int textColor, int bgColor);
};
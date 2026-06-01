#pragma once

class Motor {
private:
    bool running;

public:
    Motor();

    void start();
    void stop();

    bool isRunning() const;
};
# Industrial Conveyor Simulator

A robust, interactive console-based simulator of an industrial conveyor belt system, developed using modern C++ standards (C++20). This project serves as a practical demonstration of clean software architecture and professional programming paradigms.

## Key Features & Technologies

* **Modern C++ Memory Management:** Strict use of smart pointers (`std::unique_ptr`) and move semantics to ensure memory safety.
* **Multithreading & Synchronization:** Utilizes `std::thread` and `std::mutex` for a non-blocking Main Control Loop, allowing real-time user input via console while processing background sensor updates concurrently.
* **Software Architecture:** Built with Dependency Injection (DI) through custom interfaces (e.g., `ILogger`), making the system highly modular, testable, and easily extensible.
* **State Machine Logic:** Implements an automated state machine (`ConveyorState`) with built-in interlock safety mechanisms to handle normal operations, sensor triggers, and emergency stops securely.
* **End-to-End Scenarios:** Includes pre-programmed, automated testing scenarios to simulate real-world factory logic, automated batch processing, and conveyor jam events.

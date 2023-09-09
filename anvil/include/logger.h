#pragma once

#include <chrono>
#include <fmt/core.h>

namespace anvil {

class Logger {
public:

static Logger& instance() {
    static Logger instance;
    return instance;
}

template <typename... Args>
static void logInfo(const std::string& message, Args&&... args) {
    fmt::print(fmt::runtime(message), std::forward<Args>(args)...);
}

template <typename... Args>
static void logError(const std::string& message, Args&&... args) {
    fmt::print(stderr, fmt::runtime(message), std::forward<Args>(args)...);
}

void startTimer() {
    startTime = std::chrono::high_resolution_clock::now();
}

void endTimer(const std::string& taskName) {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    logInfo("{} took {} ms\n", taskName, duration.count());

};

private:
    std::chrono::high_resolution_clock::time_point startTime;
};

}
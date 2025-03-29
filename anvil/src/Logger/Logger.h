#pragma once

#include <vector>
#include <string>
#include <fmt/format.h>

namespace anvil {

enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
    public:
        static std::vector<LogEntry> messages;
    
        template <typename... Args>
        static void Log(fmt::format_string<Args...> fmtStr, Args&&... args) {
            LogImpl(fmt::format(fmtStr, std::forward<Args>(args)...));
        }
    
        template <typename... Args>
        static void Err(fmt::format_string<Args...> fmtStr, Args&&... args) {
            ErrImpl(fmt::format(fmtStr, std::forward<Args>(args)...));
        }

        // Legacy-style
        static void Log(const std::string& message) {
            LogImpl(message);
        }

        static void Err(const std::string& message) {
            LogImpl(message);
        }
    
    private:
        static void LogImpl(const std::string& message);
        static void ErrImpl(const std::string& message);
    };

}
#ifndef PIXELVERSE_LOGGER_HPP
#define PIXELVERSE_LOGGER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace pixelverse {
namespace utils {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    CRITICAL
};

class Logger {
public:
    static void log(LogLevel level, const std::string& message) {
        std::string tag;
        std::string colorCode;
        switch (level) {
            case LogLevel::DEBUG:    tag = "[DEBUG]"; colorCode = "\033[90m"; break;
            case LogLevel::INFO:     tag = "[INFO] "; colorCode = "\033[36m"; break;
            case LogLevel::WARNING:  tag = "[WARN] "; colorCode = "\033[33m"; break;
            case LogLevel::CRITICAL: tag = "[CRIT] "; colorCode = "\033[31m"; break;
        }

        std::cout << colorCode << tag << " \033[0m" << message << std::endl;
    }

    static void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    static void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    static void warn(const std::string& msg) { log(LogLevel::WARNING, msg); }
    static void error(const std::string& msg) { log(LogLevel::CRITICAL, msg); }
};

} // namespace utils
} // namespace pixelverse

#endif // PIXELVERSE_LOGGER_HPP

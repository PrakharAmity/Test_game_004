#ifndef PIXELVERSE_TIMEUTILS_HPP
#define PIXELVERSE_TIMEUTILS_HPP

#include <chrono>

namespace pixelverse {
namespace utils {

class TimeUtils {
public:
    static double getCurrentTimeSeconds() {
        auto now = std::chrono::steady_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration<double>(duration).count();
    }

    static long long getCurrentTimeMillis() {
        auto now = std::chrono::steady_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }
};

} // namespace utils
} // namespace pixelverse

#endif // PIXELVERSE_TIMEUTILS_HPP

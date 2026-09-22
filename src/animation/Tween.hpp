#ifndef PIXELVERSE_TWEEN_HPP
#define PIXELVERSE_TWEEN_HPP

#include "Easing.hpp"
#include <functional>

namespace pixelverse {
namespace animation {

class Tween {
public:
    double startVal;
    double endVal;
    double currentVal;
    double duration;
    double elapsed;
    EaseType ease;
    bool completed;
    std::function<void(double)> onUpdate;
    std::function<void()> onComplete;

    Tween(double start, double end, double dur, EaseType e = EaseType::QuadOut)
        : startVal(start), endVal(end), currentVal(start), duration(dur),
          elapsed(0.0), ease(e), completed(false) {}

    void update(double dt) {
        if (completed) return;
        elapsed += dt;
        double progress = elapsed / duration;
        if (progress >= 1.0) {
            progress = 1.0;
            completed = true;
        }

        double eased = Easing::evaluate(ease, progress);
        currentVal = startVal + (endVal - startVal) * eased;

        if (onUpdate) onUpdate(currentVal);
        if (completed && onComplete) onComplete();
    }

    bool isFinished() const { return completed; }
    double getValue() const { return currentVal; }
};

} // namespace animation
} // namespace pixelverse

#endif // PIXELVERSE_TWEEN_HPP

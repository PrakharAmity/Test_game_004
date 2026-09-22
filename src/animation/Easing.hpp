#ifndef PIXELVERSE_EASING_HPP
#define PIXELVERSE_EASING_HPP

#include <cmath>

namespace pixelverse {
namespace animation {

enum class EaseType {
    Linear,
    QuadIn,
    QuadOut,
    QuadInOut,
    CubicIn,
    CubicOut,
    CubicInOut,
    ElasticOut,
    BounceOut
};

class Easing {
public:
    static double evaluate(EaseType type, double t) {
        if (t <= 0.0) return 0.0;
        if (t >= 1.0) return 1.0;

        switch (type) {
            case EaseType::Linear:
                return t;
            case EaseType::QuadIn:
                return t * t;
            case EaseType::QuadOut:
                return t * (2.0 - t);
            case EaseType::QuadInOut:
                return t < 0.5 ? 2.0 * t * t : -1.0 + (4.0 - 2.0 * t) * t;
            case EaseType::CubicIn:
                return t * t * t;
            case EaseType::CubicOut: {
                double f = t - 1.0;
                return f * f * f + 1.0;
            }
            case EaseType::CubicInOut:
                return t < 0.5 ? 4.0 * t * t * t : (t - 1.0) * (2.0 * t - 2.0) * (2.0 * t - 2.0) + 1.0;
            case EaseType::ElasticOut: {
                constexpr double p = 0.3;
                return std::pow(2.0, -10.0 * t) * std::sin((t - p / 4.0) * (2.0 * 3.141592653589793) / p) + 1.0;
            }
            case EaseType::BounceOut: {
                constexpr double n1 = 7.5625;
                constexpr double d1 = 2.75;
                if (t < 1.0 / d1) {
                    return n1 * t * t;
                } else if (t < 2.0 / d1) {
                    t -= 1.5 / d1;
                    return n1 * t * t + 0.75;
                } else if (t < 2.5 / d1) {
                    t -= 2.25 / d1;
                    return n1 * t * t + 0.9375;
                } else {
                    t -= 2.625 / d1;
                    return n1 * t * t + 0.984375;
                }
            }
        }
        return t;
    }
};

} // namespace animation
} // namespace pixelverse

#endif // PIXELVERSE_EASING_HPP

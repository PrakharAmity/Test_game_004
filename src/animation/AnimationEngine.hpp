#ifndef PIXELVERSE_ANIMATIONENGINE_HPP
#define PIXELVERSE_ANIMATIONENGINE_HPP

#include "Tween.hpp"
#include <vector>
#include <memory>

namespace pixelverse {
namespace animation {

class AnimationEngine {
private:
    std::vector<std::shared_ptr<Tween>> activeTweens;

public:
    AnimationEngine() = default;

    std::shared_ptr<Tween> createTween(double start, double end, double duration, EaseType ease = EaseType::QuadOut) {
        auto tween = std::make_shared<Tween>(start, end, duration, ease);
        activeTweens.push_back(tween);
        return tween;
    }

    void update(double dt) {
        for (auto& tween : activeTweens) {
            tween->update(dt);
        }

        // Remove finished tweens
        for (auto it = activeTweens.begin(); it != activeTweens.end();) {
            if ((*it)->isFinished()) {
                it = activeTweens.erase(it);
            } else {
                ++it;
            }
        }
    }

    size_t getActiveTweenCount() const { return activeTweens.size(); }
    void clearAll() { activeTweens.clear(); }
};

} // namespace animation
} // namespace pixelverse

#endif // PIXELVERSE_ANIMATIONENGINE_HPP

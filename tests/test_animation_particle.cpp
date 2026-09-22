#include "gtest/gtest.h"
#include "../src/animation/Easing.hpp"
#include "../src/animation/AnimationEngine.hpp"
#include "../src/graphics/ParticleSystem.hpp"

using namespace pixelverse::animation;
using namespace pixelverse::graphics;

TEST(AnimationTest, Animation_EasingCubicOut) {
    // CubicOut: f(t) = (t - 1)^3 + 1
    // At t = 0: (0 - 1)^3 + 1 = 0
    EXPECT_NEAR(Easing::evaluate(EaseType::CubicOut, 0.0), 0.0, 0.0001);
    // At t = 1: (1 - 1)^3 + 1 = 1
    EXPECT_NEAR(Easing::evaluate(EaseType::CubicOut, 1.0), 1.0, 0.0001);
    // At t = 0.5: (-0.5)^3 + 1 = -0.125 + 1 = 0.875
    EXPECT_NEAR(Easing::evaluate(EaseType::CubicOut, 0.5), 0.875, 0.0001);
}

TEST(ParticleTest, Particle_LifetimeAndDecay) {
    ParticleSystem ps;
    ps.clear();
    EXPECT_EQ(ps.getActiveCount(), 0u);

    ps.emitBurst(10.0, 10.0, 5, CyberPalette::CyanGlow);
    EXPECT_EQ(ps.getActiveCount(), 5u);

    // After updating by 2.0 seconds (lifetime is between 0.5 and 1.5), all particles should decay and be removed
    ps.update(2.5);
    EXPECT_EQ(ps.getActiveCount(), 0u);
}

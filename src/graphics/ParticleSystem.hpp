#ifndef PIXELVERSE_PARTICLESYSTEM_HPP
#define PIXELVERSE_PARTICLESYSTEM_HPP

#include "ColorPalette.hpp"
#include <vector>
#include <random>

namespace pixelverse {
namespace graphics {

struct Particle {
    double x, y;
    double vx, vy;
    double life;
    double maxLife;
    Color color;
    char glyph;

    bool isAlive() const { return life > 0.0; }
};

class ParticleSystem {
private:
    std::vector<Particle> particles;
    std::mt19937 rng;

public:
    ParticleSystem();

    void emitSparkle(double x, double y, const Color& color, char glyph = '*');
    void emitBurst(double x, double y, int count, const Color& color);
    void update(double dt);
    
    const std::vector<Particle>& getParticles() const { return particles; }
    size_t getActiveCount() const { return particles.size(); }
    void clear() { particles.clear(); }
};

} // namespace graphics
} // namespace pixelverse

#endif // PIXELVERSE_PARTICLESYSTEM_HPP

#include "ParticleSystem.hpp"
#include <algorithm>

namespace pixelverse {
namespace graphics {

ParticleSystem::ParticleSystem() : rng(1337) {}

void ParticleSystem::emitSparkle(double x, double y, const Color& color, char glyph) {
    std::uniform_real_distribution<double> distV(-15.0, 15.0);
    std::uniform_real_distribution<double> distLife(0.5, 1.5);

    Particle p;
    p.x = x;
    p.y = y;
    p.vx = distV(rng);
    p.vy = distV(rng);
    p.maxLife = distLife(rng);
    p.life = p.maxLife;
    p.color = color;
    p.glyph = glyph;

    particles.push_back(p);
}

void ParticleSystem::emitBurst(double x, double y, int count, const Color& color) {
    char glyphs[] = {'*', '+', '.', 'o', '#'};
    std::uniform_int_distribution<int> glyphDist(0, 4);

    for (int i = 0; i < count; ++i) {
        emitSparkle(x, y, color, glyphs[glyphDist(rng)]);
    }
}

void ParticleSystem::update(double dt) {
    for (auto& p : particles) {
        p.x += p.vx * dt;
        p.y += p.vy * dt;
        p.life -= dt;
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(), [](const Particle& p) {
            return !p.isAlive();
        }),
        particles.end()
    );
}

} // namespace graphics
} // namespace pixelverse

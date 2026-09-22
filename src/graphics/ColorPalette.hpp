#ifndef PIXELVERSE_COLORPALETTE_HPP
#define PIXELVERSE_COLORPALETTE_HPP

#include <string>
#include <sstream>

namespace pixelverse {
namespace graphics {

struct Color {
    int r, g, b, a;

    constexpr Color(int red = 255, int green = 255, int blue = 255, int alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    std::string toAnsiForeground() const {
        std::ostringstream ss;
        ss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
        return ss.str();
    }

    std::string toAnsiBackground() const {
        std::ostringstream ss;
        ss << "\033[48;2;" << r << ";" << g << ";" << b << "m";
        return ss.str();
    }

    static std::string reset() {
        return "\033[0m";
    }
};

namespace CyberPalette {
    // Core Cyber Fantasy Palette with namespace linkage
    constexpr Color ElectricPurple{168, 85, 247};
    constexpr Color NeonBlue{59, 130, 246};
    constexpr Color CyanGlow{6, 182, 212};
    constexpr Color EmeraldGreen{16, 185, 129};
    constexpr Color RadiantGold{234, 179, 8};
    constexpr Color PinkHighlight{236, 72, 153};
    constexpr Color MatteBlack{15, 17, 23};
    constexpr Color DeepVoid{10, 10, 15};
    constexpr Color GlassSurface{30, 35, 55};
    constexpr Color GlassBorder{70, 90, 130};
    constexpr Color TextPrimary{243, 244, 246};
    constexpr Color TextSecondary{156, 163, 175};
}

} // namespace graphics
} // namespace pixelverse

#endif // PIXELVERSE_COLORPALETTE_HPP

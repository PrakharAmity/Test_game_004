#include "PlayerDashboard.hpp"

namespace pixelverse {
namespace player {

PlayerDashboard::PlayerDashboard() {
    activePlayer = Player(1, "CyberValkyrie", "avatar_valkyrie_neon", 24, 7800, 45000, 1200, 4850, 310, 8);

    // Populate initial roster
    roster.push_back(activePlayer);
    roster.push_back(Player(2, "ShadowBlade", "avatar_ninja_cyan", 19, 4200, 28000, 800, 3900, 220, 3));
    roster.push_back(Player(3, "PixelMage", "avatar_mage_purple", 27, 9100, 60000, 2100, 5200, 410, 12));
    roster.push_back(Player(4, "NeonRanger", "avatar_ranger_green", 15, 3100, 15000, 350, 2800, 160, 2));
    roster.push_back(Player(5, "QuantumTitan", "avatar_titan_gold", 30, 12000, 80000, 3500, 6100, 580, 15));
}

std::vector<Player> PlayerDashboard::filterByCoins(int minCoins, int maxCoins) const {
    std::vector<Player> matches;
    for (const auto& p : roster) {
#ifdef FIX_BUGS
        // FIX for Bug 1: Include maximum boundary condition
        if (p.coins >= minCoins && p.coins <= maxCoins) {
            matches.push_back(p);
        }
#else
        // BUG 1 (Intentional): Coin Filter Boundary Bug
        // Notice '< maxCoins' instead of '<= maxCoins'. This excludes players with exactly maxCoins.
        if (p.coins >= minCoins && p.coins < maxCoins) {
            matches.push_back(p);
        }
#endif
    }
    return matches;
}

} // namespace player
} // namespace pixelverse

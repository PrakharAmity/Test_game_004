#ifndef PIXELVERSE_PLAYERDASHBOARD_HPP
#define PIXELVERSE_PLAYERDASHBOARD_HPP

#include "Player.hpp"
#include <vector>
#include <memory>

namespace pixelverse {
namespace player {

class PlayerDashboard {
private:
    Player activePlayer;
    std::vector<Player> roster;

public:
    PlayerDashboard();

    void setActivePlayer(const Player& p) { activePlayer = p; }
    const Player& getActivePlayer() const { return activePlayer; }
    Player& getActivePlayer() { return activePlayer; }

    void addRosterPlayer(const Player& p) { roster.push_back(p); }
    const std::vector<Player>& getRoster() const { return roster; }
    void clearRoster() { roster.clear(); }

    // BUG 1 (Intentional): Coin Filter Boundary Bug
    // Filters players by their coin balances within [minCoins, maxCoins].
    // Symptom: Coin filter excludes the maximum value.
    std::vector<Player> filterByCoins(int minCoins, int maxCoins) const;
};

} // namespace player
} // namespace pixelverse

#endif // PIXELVERSE_PLAYERDASHBOARD_HPP

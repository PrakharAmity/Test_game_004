#ifndef PIXELVERSE_TERMINALNEONUI_HPP
#define PIXELVERSE_TERMINALNEONUI_HPP

#include "ColorPalette.hpp"
#include "ParticleSystem.hpp"
#include "../player/PlayerDashboard.hpp"
#include "../inventory/InventoryManager.hpp"
#include "../leaderboard/Leaderboard.hpp"
#include "../tournament/TournamentManager.hpp"
#include "../reward/RewardManager.hpp"
#include <string>

namespace pixelverse {
namespace graphics {

enum class ActiveScreen {
    Dashboard,
    Inventory,
    Leaderboard,
    Tournament,
    RewardCenter,
    Shop,
    BattleHistory
};

class TerminalNeonUI {
private:
    ActiveScreen currentScreen = ActiveScreen::Dashboard;
    ParticleSystem particleSys;
    double animationTimer = 0.0;
    int animFrame = 0;

public:
    TerminalNeonUI();

    void setActiveScreen(ActiveScreen screen) { currentScreen = screen; }
    ActiveScreen getActiveScreen() const { return currentScreen; }

    void update(double dt);

    void render(const player::PlayerDashboard& playerDash,
                const inventory::InventoryManager& inventory,
                const leaderboard::Leaderboard& leaderboard,
                const tournament::TournamentManager& tournament,
                const reward::RewardManager& rewards);

    void renderHeader();
    void renderPlayerHUD(const player::Player& player);
    void renderTabs();
    void renderDashboard(const player::Player& player);
    void renderInventory(const inventory::InventoryManager& inventory);
    void renderLeaderboard(const leaderboard::Leaderboard& leaderboard);
    void renderTournament(const tournament::TournamentManager& tournament);
    void renderRewardCenter(const reward::RewardManager& rewards);
    void renderShop(const inventory::InventoryManager& inventory);
    void renderBattleHistory(const tournament::TournamentManager& tournament);
    void renderFooter();
};

} // namespace graphics
} // namespace pixelverse

#endif // PIXELVERSE_TERMINALNEONUI_HPP

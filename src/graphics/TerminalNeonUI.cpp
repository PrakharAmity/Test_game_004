#include "TerminalNeonUI.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace pixelverse {
namespace graphics {

TerminalNeonUI::TerminalNeonUI() {
    particleSys.emitBurst(40, 10, 12, CyberPalette::CyanGlow);
}

void TerminalNeonUI::update(double dt) {
    animationTimer += dt;
    animFrame++;
    particleSys.update(dt);
    if (animFrame % 45 == 0) {
        particleSys.emitSparkle(20 + (animFrame % 40), 5, CyberPalette::ElectricPurple, '*');
    }
}

void TerminalNeonUI::renderHeader() {
    std::cout << CyberPalette::ElectricPurple.toAnsiForeground()
              << "╔════════════════════════════════════════════════════════════════════════════════════╗\n"
              << "║ " << CyberPalette::PinkHighlight.toAnsiForeground() << "⚡ PIXELVERSE LEGENDS ⚡"
              << CyberPalette::CyanGlow.toAnsiForeground() << " — AAA Cyber-Fantasy Arcade Engine v1.0.0 (C++17)   "
              << CyberPalette::ElectricPurple.toAnsiForeground() << "║\n"
              << "╚════════════════════════════════════════════════════════════════════════════════════╝\n"
              << Color::reset();
}

void TerminalNeonUI::renderPlayerHUD(const player::Player& p) {
    std::cout << CyberPalette::GlassSurface.toAnsiBackground()
              << CyberPalette::TextPrimary.toAnsiForeground()
              << "  [AVATAR: " << CyberPalette::CyanGlow.toAnsiForeground() << p.avatar << CyberPalette::TextPrimary.toAnsiForeground() << "]"
              << "  LVL " << CyberPalette::PinkHighlight.toAnsiForeground() << p.level << CyberPalette::TextPrimary.toAnsiForeground()
              << "  XP: " << p.xp << "/" << p.getRequiredXpForNextLevel()
              << "  COINS: " << CyberPalette::RadiantGold.toAnsiForeground() << p.coins << CyberPalette::TextPrimary.toAnsiForeground()
              << "  GEMS: " << CyberPalette::EmeraldGreen.toAnsiForeground() << p.gems << CyberPalette::TextPrimary.toAnsiForeground()
              << "  TROPHIES: " << CyberPalette::ElectricPurple.toAnsiForeground() << p.trophies
              << Color::reset() << "\n";

    // XP progress bar
    int barWidth = 40;
    int filled = static_cast<int>(p.getLevelProgress() * barWidth);
    std::cout << "  XP Ring: [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < filled) {
            std::cout << CyberPalette::CyanGlow.toAnsiForeground() << "█";
        } else {
            std::cout << CyberPalette::TextSecondary.toAnsiForeground() << "░";
        }
    }
    std::cout << Color::reset() << "] "
              << static_cast<int>(p.getLevelProgress() * 100) << "%\n";
}

void TerminalNeonUI::renderTabs() {
    std::cout << "\n";
    auto tab = [&](const std::string& name, ActiveScreen s) {
        if (currentScreen == s) {
            std::cout << CyberPalette::ElectricPurple.toAnsiBackground()
                      << CyberPalette::TextPrimary.toAnsiForeground()
                      << " [ " << name << " ] " << Color::reset() << " ";
        } else {
            std::cout << CyberPalette::GlassSurface.toAnsiBackground()
                      << CyberPalette::TextSecondary.toAnsiForeground()
                      << "  " << name << "  " << Color::reset() << " ";
        }
    };

    tab("1. DASHBOARD", ActiveScreen::Dashboard);
    tab("2. INVENTORY", ActiveScreen::Inventory);
    tab("3. LEADERBOARD", ActiveScreen::Leaderboard);
    tab("4. TOURNAMENT", ActiveScreen::Tournament);
    tab("5. REWARDS", ActiveScreen::RewardCenter);
    tab("6. SHOP", ActiveScreen::Shop);
    tab("7. HISTORY", ActiveScreen::BattleHistory);
    std::cout << "\n──────────────────────────────────────────────────────────────────────────────────────\n";
}

void TerminalNeonUI::renderDashboard(const player::Player& p) {
    std::cout << CyberPalette::CyanGlow.toAnsiForeground()
              << "► HERO DASHBOARD (CYBERVERSE)\n" << Color::reset();
    std::cout << "  Commander: " << p.username << " | Win Streak: " << p.winStreak << " 🔥 | Total Victories: " << p.wins << "\n";
    std::cout << "  Active Floating Island: " << CyberPalette::PinkHighlight.toAnsiForeground() << "Neo-Olympus Sky Citadel\n" << Color::reset();
    std::cout << "  Atmosphere: 60 FPS Dynamic Neon Fog | Cyber Particles Active: "
              << particleSys.getActiveCount() << "\n";
}

void TerminalNeonUI::renderInventory(const inventory::InventoryManager& inv) {
    std::cout << CyberPalette::CyanGlow.toAnsiForeground()
              << "► CYBER ARMORY & INVENTORY (" << inv.getItemCount() << " ITEMS)\n" << Color::reset();
    for (const auto& item : inv.getItems()) {
        std::string col = (item.rarity == inventory::Rarity::Mythic) ? CyberPalette::PinkHighlight.toAnsiForeground() :
                          (item.rarity == inventory::Rarity::Legendary) ? CyberPalette::RadiantGold.toAnsiForeground() :
                          (item.rarity == inventory::Rarity::Epic) ? CyberPalette::ElectricPurple.toAnsiForeground() :
                          CyberPalette::CyanGlow.toAnsiForeground();

        std::cout << "  #" << item.id << " " << col << "[" << inventory::Item::rarityToString(item.rarity) << "] "
                  << item.name << Color::reset()
                  << " | Power: " << item.power
                  << (item.equipped ? " [EQUIPPED]" : "") << "\n";
    }
}

void TerminalNeonUI::renderLeaderboard(const leaderboard::Leaderboard& lb) {
    std::cout << CyberPalette::RadiantGold.toAnsiForeground()
              << "► GLOBAL APEX LEADERBOARD (SEASON 1: NEON ASCENSION)\n" << Color::reset();
    auto ranked = lb.getRankedPlayers();
    for (const auto& entry : ranked) {
        std::string badgeColor = (entry.badge == "Gold") ? CyberPalette::RadiantGold.toAnsiForeground() :
                                 (entry.badge == "Silver") ? "\033[97m" :
                                 (entry.badge == "Bronze") ? CyberPalette::PinkHighlight.toAnsiForeground() :
                                 CyberPalette::CyanGlow.toAnsiForeground();

        std::cout << "  Rank #" << entry.rank << " " << badgeColor << "[" << entry.badge << "] "
                  << entry.username << Color::reset()
                  << " - Score: " << entry.score << " pts | Wins: " << entry.wins
                  << " | Streak: " << entry.winStreak << " 🔥\n";
    }
}

void TerminalNeonUI::renderTournament(const tournament::TournamentManager& tourney) {
    std::cout << CyberPalette::ElectricPurple.toAnsiForeground()
              << "► TOURNAMENT ARENA — NEON BRACKET (PAGE 1 OF 2)\n" << Color::reset();
    auto pageMatches = tourney.getMatchesPage(1, 4);
    for (const auto& m : pageMatches) {
        std::cout << "  Match #" << m.id << " [" << m.stage << "] "
                  << m.playerOne << " (" << m.scoreOne << ") vs (" << m.scoreTwo << ") " << m.playerTwo
                  << " ► Winner: " << CyberPalette::EmeraldGreen.toAnsiForeground() << m.winner << Color::reset() << "\n";
    }
}

void TerminalNeonUI::renderRewardCenter(const reward::RewardManager& rewards) {
    std::cout << CyberPalette::RadiantGold.toAnsiForeground()
              << "► REWARD VAULT & TREASURE CHESTS (Pending: " << rewards.getQueueSize() << ")\n" << Color::reset();
    auto list = rewards.getQueueAsVector();
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << "  Queue [" << i + 1 << "] "
                  << (list[i].isPriority ? CyberPalette::PinkHighlight.toAnsiForeground() + "★ VIP PRIORITY ★ " : "")
                  << list[i].title << Color::reset()
                  << " — Coins: +" << list[i].coins << " | Gems: +" << list[i].gems
                  << (list[i].bonusItem.empty() ? "" : " | Loot: " + list[i].bonusItem) << "\n";
    }
}

void TerminalNeonUI::renderShop(const inventory::InventoryManager& inv) {
    std::cout << CyberPalette::PinkHighlight.toAnsiForeground()
              << "► NEON CYBER SHOP — FEATURED SKINS & ARSENAL\n" << Color::reset();
    std::cout << "  * Chrono Valkyrie (Mythic Skin) — 12,000 Coins / 400 Gems [PREVIEW AVAILABLE]\n";
    std::cout << "  * Cyber Void Bow (Epic Weapon)   — 3,200 Coins / 80 Gems\n";
    std::cout << "  * Mecha-Dragon Companion (Pet)   — 20,000 Coins / 600 Gems\n";
}

void TerminalNeonUI::renderBattleHistory(const tournament::TournamentManager& tourney) {
    std::cout << CyberPalette::CyanGlow.toAnsiForeground()
              << "► BATTLE HISTORY TIMELINE\n" << Color::reset();
    for (size_t i = 0; i < 4 && i < tourney.getAllMatches().size(); ++i) {
        const auto& m = tourney.getAllMatches()[i];
        std::cout << "  Battle #" << m.id << " - " << m.stage
                  << " | " << CyberPalette::EmeraldGreen.toAnsiForeground() << "VICTORY" << Color::reset()
                  << " by " << m.winner << " (" << m.scoreOne << "-" << m.scoreTwo << ")\n";
    }
}

void TerminalNeonUI::renderFooter() {
    std::cout << "\n" << CyberPalette::TextSecondary.toAnsiForeground()
              << "[Press 1-7 to switch tabs, 'C' to claim reward, 'Q' to quit]\n"
              << Color::reset();
}

void TerminalNeonUI::render(const player::PlayerDashboard& playerDash,
                            const inventory::InventoryManager& inventory,
                            const leaderboard::Leaderboard& leaderboard,
                            const tournament::TournamentManager& tournament,
                            const reward::RewardManager& rewards) {
    renderHeader();
    renderPlayerHUD(playerDash.getActivePlayer());
    renderTabs();

    switch (currentScreen) {
        case ActiveScreen::Dashboard:    renderDashboard(playerDash.getActivePlayer()); break;
        case ActiveScreen::Inventory:    renderInventory(inventory); break;
        case ActiveScreen::Leaderboard:  renderLeaderboard(leaderboard); break;
        case ActiveScreen::Tournament:   renderTournament(tournament); break;
        case ActiveScreen::RewardCenter: renderRewardCenter(rewards); break;
        case ActiveScreen::Shop:         renderShop(inventory); break;
        case ActiveScreen::BattleHistory:renderBattleHistory(tournament); break;
    }

    renderFooter();
}

} // namespace graphics
} // namespace pixelverse

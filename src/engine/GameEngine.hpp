#ifndef PIXELVERSE_GAMEENGINE_HPP
#define PIXELVERSE_GAMEENGINE_HPP

#include "StateMachine.hpp"
#include "../animation/AnimationEngine.hpp"
#include "../audio/SoundEngine.hpp"
#include "../player/PlayerDashboard.hpp"
#include "../inventory/InventoryManager.hpp"
#include "../leaderboard/Leaderboard.hpp"
#include "../tournament/TournamentManager.hpp"
#include "../reward/RewardManager.hpp"
#include "../graphics/TerminalNeonUI.hpp"
#include <memory>
#include <string>

namespace pixelverse {
namespace engine {

class GameEngine {
private:
    StateMachine stateMachine;
    animation::AnimationEngine animationEngine;
    audio::SoundEngine soundEngine;
    player::PlayerDashboard playerDashboard;
    inventory::InventoryManager inventoryManager;
    leaderboard::Leaderboard leaderboard;
    tournament::TournamentManager tournamentManager;
    reward::RewardManager rewardManager;
    graphics::TerminalNeonUI neonUI;

    bool isRunning = false;
    double targetFps = 60.0;
    std::string saveFilePath = "save/player_save.json";

public:
    GameEngine();

    void initialize();
    void update(double dt);
    void render();

    bool saveGame(const std::string& path = "");
    bool loadGame(const std::string& path = "");

    void claimReward();

    StateMachine& getStateMachine() { return stateMachine; }
    animation::AnimationEngine& getAnimationEngine() { return animationEngine; }
    audio::SoundEngine& getSoundEngine() { return soundEngine; }
    player::PlayerDashboard& getPlayerDashboard() { return playerDashboard; }
    inventory::InventoryManager& getInventoryManager() { return inventoryManager; }
    leaderboard::Leaderboard& getLeaderboard() { return leaderboard; }
    tournament::TournamentManager& getTournamentManager() { return tournamentManager; }
    reward::RewardManager& getRewardManager() { return rewardManager; }
    graphics::TerminalNeonUI& getUI() { return neonUI; }

    bool running() const { return isRunning; }
    void stop() { isRunning = false; }
};

} // namespace engine
} // namespace pixelverse

#endif // PIXELVERSE_GAMEENGINE_HPP

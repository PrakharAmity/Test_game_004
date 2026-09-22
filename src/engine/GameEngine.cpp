#include "GameEngine.hpp"
#include "../utils/Logger.hpp"
#include "../utils/JsonHelper.hpp"
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace pixelverse {
namespace engine {

GameEngine::GameEngine() : soundEngine("assets/audio") {}

void GameEngine::initialize() {
    utils::Logger::info("Initializing PixelVerse Legends Game Engine...");

#ifdef _WIN32
    CreateDirectoryA("save", NULL);
    CreateDirectoryA("assets", NULL);
    CreateDirectoryA("assets/audio", NULL);
#endif

    soundEngine.initializeOfflineAssets();
    stateMachine.transitionTo(EngineState::MainMenu);
    isRunning = true;

    utils::Logger::info("Game Engine initialized at 60 FPS target.");
}

void GameEngine::update(double dt) {
    animationEngine.update(dt);
    neonUI.update(dt);
}

void GameEngine::render() {
    neonUI.render(playerDashboard, inventoryManager, leaderboard, tournamentManager, rewardManager);
}

void GameEngine::claimReward() {
    reward::Reward rew;
    if (rewardManager.claimNextReward(rew)) {
        auto& p = playerDashboard.getActivePlayer();
        p.coins += rew.coins;
        p.gems += rew.gems;
        p.addXp(rew.xp);
        soundEngine.playSound(audio::SoundEvent::RewardUnlock);
        utils::Logger::info("Reward Claimed: " + rew.title + " (+" + std::to_string(rew.coins) + " coins, +" + std::to_string(rew.gems) + " gems)");
    } else {
        soundEngine.playSound(audio::SoundEvent::Error);
        utils::Logger::warn("No rewards remaining in claim queue!");
    }
}

bool GameEngine::saveGame(const std::string& path) {
    std::string target = path.empty() ? saveFilePath : path;
    utils::JsonValue root;
    root["player"] = playerDashboard.getActivePlayer().toJson();
    root["inventory"] = inventoryManager.toJson();
    root["season"] = "Season 1: Neon Ascension";

    bool success = utils::JsonHelper::saveToFile(target, root);
    if (success) {
        utils::Logger::info("Game state saved successfully to: " + target);
    } else {
        utils::Logger::error("Failed to save game state to: " + target);
    }
    return success;
}

bool GameEngine::loadGame(const std::string& path) {
    std::string target = path.empty() ? saveFilePath : path;
    utils::JsonValue root = utils::JsonHelper::loadFromFile(target);
    if (root.isNull() || !root.isObject()) {
        utils::Logger::warn("No valid save file found at: " + target);
        return false;
    }

    if (root.contains("player")) {
        playerDashboard.setActivePlayer(player::Player::fromJson(root["player"]));
    }
    if (root.contains("inventory")) {
        inventoryManager.loadFromJson(root["inventory"]);
    }

    utils::Logger::info("Game state loaded from: " + target);
    return true;
}

} // namespace engine
} // namespace pixelverse

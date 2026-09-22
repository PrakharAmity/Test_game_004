#include "StateMachine.hpp"
#include "../utils/Logger.hpp"

namespace pixelverse {
namespace engine {

void StateMachine::transitionTo(EngineState newState) {
    if (newState == currentState) return;
    previousState = currentState;
    currentState = newState;
    utils::Logger::info("Engine State Transition: " + stateToString(previousState) + " -> " + stateToString(currentState));
}

std::string StateMachine::stateToString(EngineState state) {
    switch (state) {
        case EngineState::Boot: return "Boot";
        case EngineState::MainMenu: return "MainMenu";
        case EngineState::Lobby: return "Lobby";
        case EngineState::Matchmaking: return "Matchmaking";
        case EngineState::InBattle: return "InBattle";
        case EngineState::RewardClaim: return "RewardClaim";
        case EngineState::Paused: return "Paused";
        case EngineState::Shutdown: return "Shutdown";
    }
    return "Unknown";
}

} // namespace engine
} // namespace pixelverse

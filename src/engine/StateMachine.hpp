#ifndef PIXELVERSE_STATEMACHINE_HPP
#define PIXELVERSE_STATEMACHINE_HPP

#include <string>
#include <memory>
#include <map>

namespace pixelverse {
namespace engine {

enum class EngineState {
    Boot,
    MainMenu,
    Lobby,
    Matchmaking,
    InBattle,
    RewardClaim,
    Paused,
    Shutdown
};

class StateMachine {
private:
    EngineState currentState = EngineState::Boot;
    EngineState previousState = EngineState::Boot;

public:
    StateMachine() = default;

    void transitionTo(EngineState newState);
    EngineState getCurrentState() const { return currentState; }
    EngineState getPreviousState() const { return previousState; }
    static std::string stateToString(EngineState state);
};

} // namespace engine
} // namespace pixelverse

#endif // PIXELVERSE_STATEMACHINE_HPP

cmake_minimum_required(VERSION 3.14)
project(PixelVerse-Legends VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option to toggle fixes for testing and challenge evaluation
option(FIX_BUGS "Compile with all 4 logical bug fixes applied" OFF)

if(FIX_BUGS)
    message(STATUS ">> Compiling PixelVerse Legends with FIX_BUGS enabled")
    add_compile_definitions(FIX_BUGS)
else()
    message(STATUS ">> Compiling PixelVerse Legends in CHALLENGE MODE (4 intentional bugs present)")
endif()

# Compiler flags - Optimized for low resources and minimal binary size
if(MSVC)
    add_compile_options(/W4 /EHsc /O2 /Gy)
else()
    add_compile_options(-Wall -Wextra -pedantic -Os -ffunction-sections -fdata-sections)
    add_link_options(-Wl,--gc-sections)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# Core engine source files
set(ENGINE_SOURCES
    src/utils/Logger.hpp
    src/utils/TimeUtils.hpp
    src/utils/JsonHelper.hpp
    src/utils/JsonHelper.cpp
    src/animation/Easing.hpp
    src/animation/Tween.hpp
    src/animation/AnimationEngine.hpp
    src/animation/AnimationEngine.cpp
    src/graphics/ColorPalette.hpp
    src/graphics/ParticleSystem.hpp
    src/graphics/ParticleSystem.cpp
    src/graphics/TerminalNeonUI.hpp
    src/graphics/TerminalNeonUI.cpp
    src/audio/WavGenerator.hpp
    src/audio/SoundEngine.hpp
    src/audio/SoundEngine.cpp
    src/player/Player.hpp
    src/player/PlayerDashboard.hpp
    src/player/PlayerDashboard.cpp
    src/tournament/Match.hpp
    src/tournament/TournamentManager.hpp
    src/tournament/TournamentManager.cpp
    src/leaderboard/Leaderboard.hpp
    src/leaderboard/Leaderboard.cpp
    src/inventory/Item.hpp
    src/inventory/InventoryManager.hpp
    src/inventory/InventoryManager.cpp
    src/reward/Reward.hpp
    src/reward/RewardManager.hpp
    src/reward/RewardManager.cpp
    src/engine/StateMachine.hpp
    src/engine/StateMachine.cpp
    src/engine/GameEngine.hpp
    src/engine/GameEngine.cpp
)

add_library(pixelverse_core STATIC ${ENGINE_SOURCES})

if(WIN32)
    target_link_libraries(pixelverse_core PUBLIC winmm ws2_32 crypt32)
endif()

# Main Interactive / Demo Game Application
add_executable(pixelverse_app src/main.cpp)
target_link_libraries(pixelverse_app PRIVATE pixelverse_core)

# GoogleTest Test Suite (36 Deterministic Tests)
set(TEST_SOURCES
    tests/gtest/gtest.h
    tests/gtest/gtest.cpp
    tests/test_player_filter.cpp
    tests/test_tournament_pagination.cpp
    tests/test_leaderboard_sorting.cpp
    tests/test_inventory_search.cpp
    tests/test_reward_queue.cpp
    tests/test_inventory_removal.cpp
    tests/test_save_load.cpp
    tests/test_animation_particle.cpp
    tests/main_test.cpp
)

add_executable(pixelverse_tests ${TEST_SOURCES})
target_link_libraries(pixelverse_tests PRIVATE pixelverse_core)

# Section 12 Deterministic JSON Challenge Evaluator
add_executable(pixelverse_evaluator tests/challenge_evaluator.cpp)
target_link_libraries(pixelverse_evaluator PRIVATE pixelverse_core)

enable_testing()
add_test(NAME PixelVerseGoogleTests COMMAND pixelverse_tests)
add_test(NAME PixelVerseChallengeEvaluator COMMAND pixelverse_evaluator)

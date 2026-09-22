#include "gtest/gtest.h"
#include "../src/player/PlayerDashboard.hpp"

using namespace pixelverse::player;

TEST(PlayerFilterTest, FilterByCoins_InclusiveRange) {
    PlayerDashboard dash;
    dash.clearRoster();
    dash.addRosterPlayer(Player(1, "Alpha", "av1", 10, 100, 1000, 10, 100, 10, 1));
    dash.addRosterPlayer(Player(2, "Beta", "av2", 10, 100, 2000, 10, 100, 10, 1));
    dash.addRosterPlayer(Player(3, "Gamma", "av3", 10, 100, 3000, 10, 100, 10, 1));

    // Target: Range [1000, 3000] should return all 3 players
    // Bug 1 will exclude Gamma (3000) because it uses '< 3000'
    auto result = dash.filterByCoins(1000, 3000);
    EXPECT_EQ(result.size(), 3u);
}

TEST(PlayerFilterTest, FilterByCoins_ExactMaxCoin) {
    PlayerDashboard dash;
    dash.clearRoster();
    dash.addRosterPlayer(Player(10, "Apex", "av1", 20, 500, 50000, 50, 500, 50, 5));

    // Filter with min=10000, max=50000. Player has exactly 50000 coins.
    auto result = dash.filterByCoins(10000, 50000);
    EXPECT_EQ(result.size(), 1u);
}

TEST(PlayerFilterTest, FilterByCoins_ExactMinCoin) {
    PlayerDashboard dash;
    dash.clearRoster();
    dash.addRosterPlayer(Player(10, "Apex", "av1", 20, 500, 50000, 50, 500, 50, 5));

    // Filter with min=50000, max=60000. Player has exactly 50000 coins.
    auto result = dash.filterByCoins(50000, 60000);
    EXPECT_EQ(result.size(), 1u);
}

TEST(PlayerFilterTest, FilterByCoins_EmptyResult) {
    PlayerDashboard dash;
    dash.clearRoster();
    dash.addRosterPlayer(Player(1, "Player1", "av1", 10, 100, 500, 10, 100, 10, 1));

    // Out of range [2000, 4000]
    auto result = dash.filterByCoins(2000, 4000);
    EXPECT_EQ(result.size(), 0u);
}

TEST(PlayerFilterTest, FilterByCoins_SinglePlayerMatch) {
    PlayerDashboard dash;
    dash.clearRoster();
    dash.addRosterPlayer(Player(1, "Player1", "av1", 10, 100, 1500, 10, 100, 10, 1));
    dash.addRosterPlayer(Player(2, "Player2", "av2", 10, 100, 4500, 10, 100, 10, 1));

    // Range [1000, 2000] matches Player1 strictly inside boundary
    auto result = dash.filterByCoins(1000, 2000);
    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].username, "Player1");
}

TEST(PlayerFilterTest, PlayerProfile_XpToLevelCalculation) {
    Player p(1, "Hero", "av", 1, 0, 0, 0, 0, 0, 0);
    EXPECT_EQ(p.getRequiredXpForNextLevel(), 1000);
    p.addXp(1200); // 1000 reaches lvl 2, remainder 200 xp
    EXPECT_EQ(p.level, 2);
    EXPECT_EQ(p.xp, 200);
    EXPECT_EQ(p.getRequiredXpForNextLevel(), 2000);
}

#include "gtest/gtest.h"
#include "../src/leaderboard/Leaderboard.hpp"

using namespace pixelverse::leaderboard;

TEST(LeaderboardTest, Leaderboard_DescendingScoreOrder) {
    Leaderboard lb;
    auto ranked = lb.getRankedPlayers();
    ASSERT_GE(ranked.size(), 2u);

    // Highest score is QuantumTitan (6100), lowest is NeonRanger (2800)
    // With Bug 3, ascending sort puts NeonRanger at Rank 1 (index 0)
    EXPECT_EQ(ranked[0].username, "QuantumTitan");
    EXPECT_EQ(ranked[0].score, 6100);
}

TEST(LeaderboardTest, Leaderboard_TieBreakerByWins) {
    Leaderboard lb;
    lb.clear();
    // Two players with equal score of 5000:
    // Player A: winStreak 10
    // Player B: winStreak 2
    lb.addEntry(LeaderboardEntry(0, 1, "PlayerLowStreak", "av", 5000, 50, 2));
    lb.addEntry(LeaderboardEntry(0, 2, "PlayerHighStreak", "av", 5000, 50, 10));

    auto ranked = lb.getRankedPlayers();
    ASSERT_EQ(ranked.size(), 2u);
    // Player with higher streak should rank first
    EXPECT_EQ(ranked[0].username, "PlayerHighStreak");
}

TEST(LeaderboardTest, Leaderboard_TopThreePodium) {
    Leaderboard lb;
    auto ranked = lb.getRankedPlayers();
    ASSERT_GE(ranked.size(), 3u);

    EXPECT_EQ(ranked[0].badge, "Gold");
    EXPECT_EQ(ranked[1].badge, "Silver");
    EXPECT_EQ(ranked[2].badge, "Bronze");

    // Verify scores are non-increasing: score[0] >= score[1] >= score[2]
    EXPECT_GE(ranked[0].score, ranked[1].score);
    EXPECT_GE(ranked[1].score, ranked[2].score);
}

TEST(LeaderboardTest, Leaderboard_EmptyLeaderboard) {
    Leaderboard lb;
    lb.clear();
    auto ranked = lb.getRankedPlayers();
    EXPECT_EQ(ranked.size(), 0u);
}

TEST(LeaderboardTest, Leaderboard_SinglePlayer) {
    Leaderboard lb;
    lb.clear();
    lb.addEntry(LeaderboardEntry(0, 99, "LoneWolf", "av", 1200, 10, 1));
    auto ranked = lb.getRankedPlayers();
    ASSERT_EQ(ranked.size(), 1u);
    EXPECT_EQ(ranked[0].rank, 1);
    EXPECT_EQ(ranked[0].badge, "Gold");
}

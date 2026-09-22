#include "gtest/gtest.h"
#include "../src/tournament/TournamentManager.hpp"

using namespace pixelverse::tournament;

TEST(TournamentPaginationTest, Pagination_PageTwoFirstItem) {
    TournamentManager tourney;
    // 16 total matches, IDs 1 to 16
    size_t pageSize = 4;
    // Page 2 should have 4 matches, starting with Match #5 (0-indexed 4)
    auto page2 = tourney.getMatchesPage(2, pageSize);
    ASSERT_EQ(page2.size(), 4u);
    EXPECT_EQ(page2[0].id, 5); // With Bug 2, page2[0] will be Match #6 (id 6) because index 4 was skipped!
}

TEST(TournamentPaginationTest, Pagination_PageOneBoundary) {
    TournamentManager tourney;
    size_t pageSize = 4;
    auto page1 = tourney.getMatchesPage(1, pageSize);
    ASSERT_EQ(page1.size(), 4u);
    EXPECT_EQ(page1[0].id, 1);
    EXPECT_EQ(page1[3].id, 4);
}

TEST(TournamentPaginationTest, Pagination_ConsecutivePagesNoOverlap) {
    TournamentManager tourney;
    size_t pageSize = 4;
    auto page1 = tourney.getMatchesPage(1, pageSize);
    auto page2 = tourney.getMatchesPage(2, pageSize);
    ASSERT_EQ(page1.size(), 4u);
    ASSERT_EQ(page2.size(), 4u);
    // End of page 1 is ID 4, start of page 2 should be ID 5
    EXPECT_EQ(page1[3].id + 1, page2[0].id);
}

TEST(TournamentPaginationTest, Pagination_LastPagePartial) {
    TournamentManager tourney;
    tourney.clearMatches();
    // 7 matches with pageSize 5 => Page 2 has 2 matches
    for (int i = 1; i <= 7; ++i) {
        tourney.addMatch(Match(i, "P1", "P2", 2, 0, "Quarter", MatchStatus::Completed, "P1"));
    }
    auto page2 = tourney.getMatchesPage(2, 5);
    // Page 2 should have remaining 2 items (IDs 6 and 7)
    // In buggy version, startIndex is (2-1)*5 + 1 = 6, so only item 7 is returned!
    EXPECT_EQ(page2.size(), 2u);
}

TEST(TournamentPaginationTest, Pagination_OutOfBoundsPage) {
    TournamentManager tourney;
    auto page99 = tourney.getMatchesPage(99, 4);
    EXPECT_EQ(page99.size(), 0u);
}

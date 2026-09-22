#include "Leaderboard.hpp"
#include <algorithm>

namespace pixelverse {
namespace leaderboard {

Leaderboard::Leaderboard(const std::string& season) : seasonName(season) {
    entries.push_back(LeaderboardEntry(0, 101, "QuantumTitan", "avatar_titan_gold", 6100, 580, 15, "Master"));
    entries.push_back(LeaderboardEntry(0, 102, "PixelMage", "avatar_mage_purple", 5200, 410, 12, "Diamond"));
    entries.push_back(LeaderboardEntry(0, 103, "CyberValkyrie", "avatar_valkyrie_neon", 4850, 310, 8, "Diamond"));
    entries.push_back(LeaderboardEntry(0, 104, "ShadowBlade", "avatar_ninja_cyan", 3900, 220, 3, "Gold"));
    entries.push_back(LeaderboardEntry(0, 105, "NeonRanger", "avatar_ranger_green", 2800, 160, 2, "Silver"));
    entries.push_back(LeaderboardEntry(0, 106, "GlitchStalker", "avatar_glitch_pink", 4850, 290, 5, "Diamond")); // Same score as CyberValkyrie
}

std::vector<LeaderboardEntry> Leaderboard::getRankedPlayers() const {
    std::vector<LeaderboardEntry> sorted = entries;

#ifdef FIX_BUGS
    // FIX for Bug 3: Correct descending comparator with tie-breaker
    std::sort(sorted.begin(), sorted.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        if (a.score != b.score) {
            return a.score > b.score; // Highest score first
        }
        if (a.winStreak != b.winStreak) {
            return a.winStreak > b.winStreak; // Highest win streak next
        }
        return a.wins > b.wins;
    });
#else
    // BUG 3 (Intentional): Leaderboard Ranking Bug (DSA)
    // Concept: Sorting Comparator with std::sort
    // Symptom: Ranks players with lowest score at the top (Ascending instead of Descending)
    std::sort(sorted.begin(), sorted.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        if (a.score != b.score) {
            return a.score < b.score;
        }
        return a.winStreak < b.winStreak;
    });
#endif

    // Assign podium badges & ranks
    for (size_t i = 0; i < sorted.size(); ++i) {
        sorted[i].rank = static_cast<int>(i + 1);
        if (sorted[i].rank == 1) sorted[i].badge = "Gold";
        else if (sorted[i].rank == 2) sorted[i].badge = "Silver";
        else if (sorted[i].rank == 3) sorted[i].badge = "Bronze";
    }

    return sorted;
}

} // namespace leaderboard
} // namespace pixelverse

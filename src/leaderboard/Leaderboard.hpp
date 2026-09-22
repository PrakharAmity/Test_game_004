#ifndef PIXELVERSE_LEADERBOARD_HPP
#define PIXELVERSE_LEADERBOARD_HPP

#include <string>
#include <vector>
#include "../utils/JsonHelper.hpp"

namespace pixelverse {
namespace leaderboard {

struct LeaderboardEntry {
    int rank;
    int playerId;
    std::string username;
    std::string avatar;
    int score;
    int wins;
    int winStreak;
    std::string badge; // "Gold", "Silver", "Bronze", "Diamond", "Master"

    LeaderboardEntry(int r = 0, int pid = 0, const std::string& name = "",
                     const std::string& av = "", int sc = 0, int w = 0, int streak = 0,
                     const std::string& b = "Contender")
        : rank(r), playerId(pid), username(name), avatar(av),
          score(sc), wins(w), winStreak(streak), badge(b) {}

    utils::JsonValue toJson() const {
        utils::JsonValue json;
        json["rank"] = rank;
        json["playerId"] = playerId;
        json["username"] = username;
        json["avatar"] = avatar;
        json["score"] = score;
        json["wins"] = wins;
        json["winStreak"] = winStreak;
        json["badge"] = badge;
        return json;
    }
};

class Leaderboard {
private:
    std::string seasonName;
    std::vector<LeaderboardEntry> entries;

public:
    Leaderboard(const std::string& season = "Season 1: Neon Ascension");

    void addEntry(const LeaderboardEntry& entry) { entries.push_back(entry); }
    void clear() { entries.clear(); }
    const std::vector<LeaderboardEntry>& getRawEntries() const { return entries; }

    // BUG 3 (Intentional): Leaderboard Ranking Bug (DSA)
    // Concept: Sorting Comparator with std::sort
    // Symptom: Leaderboard ranks players incorrectly.
    std::vector<LeaderboardEntry> getRankedPlayers() const;
};

} // namespace leaderboard
} // namespace pixelverse

#endif // PIXELVERSE_LEADERBOARD_HPP

#include "TournamentManager.hpp"

namespace pixelverse {
namespace tournament {

TournamentManager::TournamentManager(const std::string& name) : tournamentName(name) {
    // Populate realistic tournament bracket match history (16 matches)
    for (int i = 1; i <= 16; ++i) {
        std::string p1 = "Gladiator_" + std::to_string(i * 2 - 1);
        std::string p2 = "Gladiator_" + std::to_string(i * 2);
        std::string stage = (i <= 8) ? "Round of 16" : ((i <= 12) ? "Quarter-Final" : ((i <= 14) ? "Semi-Final" : "Grand Final"));
        std::string win = (i % 2 == 0) ? p1 : p2;
        allMatches.push_back(Match(i, p1, p2, 3, (i % 3), stage, MatchStatus::Completed, win));
    }
}

std::vector<Match> TournamentManager::getMatchesPage(size_t page, size_t pageSize) const {
    std::vector<Match> pageMatches;
    if (page == 0 || pageSize == 0) return pageMatches;

#ifdef FIX_BUGS
    // FIX for Bug 2: Proper 0-indexed pagination offset
    size_t startIndex = (page - 1) * pageSize;
#else
    // BUG 2 (Intentional): Tournament Pagination Bug
    // Notice that for page > 1, startIndex skips 1 match ((page - 1) * pageSize + 1)!
    size_t startIndex = (page == 1) ? 0 : ((page - 1) * pageSize + 1);
#endif

    if (startIndex >= allMatches.size()) return pageMatches;

    for (size_t i = startIndex; i < startIndex + pageSize && i < allMatches.size(); ++i) {
        pageMatches.push_back(allMatches[i]);
    }
    return pageMatches;
}

} // namespace tournament
} // namespace pixelverse

#ifndef PIXELVERSE_TOURNAMENTMANAGER_HPP
#define PIXELVERSE_TOURNAMENTMANAGER_HPP

#include "Match.hpp"
#include <vector>

namespace pixelverse {
namespace tournament {

class TournamentManager {
private:
    std::string tournamentName;
    std::vector<Match> allMatches;

public:
    TournamentManager(const std::string& name = "Neon Championship Series");

    void addMatch(const Match& m) { allMatches.push_back(m); }
    const std::vector<Match>& getAllMatches() const { return allMatches; }
    size_t getTotalMatches() const { return allMatches.size(); }
    void clearMatches() { allMatches.clear(); }

    // BUG 2 (Intentional): Tournament Pagination Bug
    // Returns matches for the requested 1-based page and pageSize.
    // Symptom: Every tournament page after the first skips one match.
    std::vector<Match> getMatchesPage(size_t page, size_t pageSize) const;

    size_t getTotalPages(size_t pageSize) const {
        if (pageSize == 0) return 0;
        return (allMatches.size() + pageSize - 1) / pageSize;
    }
};

} // namespace tournament
} // namespace pixelverse

#endif // PIXELVERSE_TOURNAMENTMANAGER_HPP

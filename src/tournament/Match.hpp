#ifndef PIXELVERSE_MATCH_HPP
#define PIXELVERSE_MATCH_HPP

#include <string>
#include "../utils/JsonHelper.hpp"

namespace pixelverse {
namespace tournament {

enum class MatchStatus {
    Scheduled,
    Live,
    Completed
};

struct Match {
    int id;
    std::string playerOne;
    std::string playerTwo;
    int scoreOne;
    int scoreTwo;
    std::string stage; // "Quarter-Final", "Semi-Final", "Grand Final"
    MatchStatus status;
    std::string winner;

    Match(int matchId = 1,
          const std::string& p1 = "CyberValkyrie",
          const std::string& p2 = "ShadowBlade",
          int s1 = 3,
          int s2 = 1,
          const std::string& stg = "Round of 16",
          MatchStatus stat = MatchStatus::Completed,
          const std::string& win = "CyberValkyrie")
        : id(matchId), playerOne(p1), playerTwo(p2),
          scoreOne(s1), scoreTwo(s2), stage(stg),
          status(stat), winner(win) {}

    utils::JsonValue toJson() const {
        utils::JsonValue json;
        json["id"] = id;
        json["playerOne"] = playerOne;
        json["playerTwo"] = playerTwo;
        json["scoreOne"] = scoreOne;
        json["scoreTwo"] = scoreTwo;
        json["stage"] = stage;
        json["status"] = static_cast<int>(status);
        json["winner"] = winner;
        return json;
    }
};

} // namespace tournament
} // namespace pixelverse

#endif // PIXELVERSE_MATCH_HPP

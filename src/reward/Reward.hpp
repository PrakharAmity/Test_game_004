#ifndef PIXELVERSE_REWARD_HPP
#define PIXELVERSE_REWARD_HPP

#include <string>
#include "../utils/JsonHelper.hpp"

namespace pixelverse {
namespace reward {

enum class RewardType {
    DailyLogin,
    WeeklyTrophy,
    TournamentBounty,
    TreasureChest,
    VIPSpecial
};

struct Reward {
    int id;
    std::string title;
    RewardType type;
    int coins;
    int gems;
    int xp;
    std::string bonusItem;
    bool isPriority;

    Reward(int rId = 0, const std::string& t = "", RewardType rType = RewardType::DailyLogin,
           int c = 100, int g = 10, int x = 50, const std::string& item = "", bool priority = false)
        : id(rId), title(t), type(rType), coins(c), gems(g),
          xp(x), bonusItem(item), isPriority(priority) {}

    utils::JsonValue toJson() const {
        utils::JsonValue json;
        json["id"] = id;
        json["title"] = title;
        json["coins"] = coins;
        json["gems"] = gems;
        json["xp"] = xp;
        json["bonusItem"] = bonusItem;
        json["isPriority"] = isPriority;
        return json;
    }
};

} // namespace reward
} // namespace pixelverse

#endif // PIXELVERSE_REWARD_HPP

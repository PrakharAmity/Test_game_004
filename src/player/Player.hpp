#ifndef PIXELVERSE_PLAYER_HPP
#define PIXELVERSE_PLAYER_HPP

#include <string>
#include "../utils/JsonHelper.hpp"

namespace pixelverse {
namespace player {

class Player {
public:
    int id;
    std::string username;
    std::string avatar;
    int level;
    int xp;
    int coins;
    int gems;
    int trophies;
    int wins;
    int winStreak;

    Player(int playerId = 1,
           const std::string& name = "NeonKnight",
           const std::string& av = "cyber_hero_01",
           int lvl = 10,
           int exp = 4500,
           int c = 12500,
           int g = 450,
           int troph = 3200,
           int w = 142,
           int streak = 5)
        : id(playerId), username(name), avatar(av), level(lvl),
          xp(exp), coins(c), gems(g), trophies(troph),
          wins(w), winStreak(streak) {}

    utils::JsonValue toJson() const {
        utils::JsonValue json;
        json["id"] = id;
        json["username"] = username;
        json["avatar"] = avatar;
        json["level"] = level;
        json["xp"] = xp;
        json["coins"] = coins;
        json["gems"] = gems;
        json["trophies"] = trophies;
        json["wins"] = wins;
        json["winStreak"] = winStreak;
        return json;
    }

    static Player fromJson(const utils::JsonValue& json) {
        Player p;
        if (json.contains("id")) p.id = json["id"].asInt();
        if (json.contains("username")) p.username = json["username"].asString();
        if (json.contains("avatar")) p.avatar = json["avatar"].asString();
        if (json.contains("level")) p.level = json["level"].asInt();
        if (json.contains("xp")) p.xp = json["xp"].asInt();
        if (json.contains("coins")) p.coins = json["coins"].asInt();
        if (json.contains("gems")) p.gems = json["gems"].asInt();
        if (json.contains("trophies")) p.trophies = json["trophies"].asInt();
        if (json.contains("wins")) p.wins = json["wins"].asInt();
        if (json.contains("winStreak")) p.winStreak = json["winStreak"].asInt();
        return p;
    }

    void addXp(int amount) {
        xp += amount;
        while (xp >= getRequiredXpForNextLevel()) {
            xp -= getRequiredXpForNextLevel();
            level++;
        }
    }

    int getRequiredXpForNextLevel() const {
        return level * 1000;
    }

    double getLevelProgress() const {
        int req = getRequiredXpForNextLevel();
        if (req <= 0) return 1.0;
        return static_cast<double>(xp) / static_cast<double>(req);
    }
};

} // namespace player
} // namespace pixelverse

#endif // PIXELVERSE_PLAYER_HPP

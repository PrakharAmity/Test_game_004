#ifndef PIXELVERSE_ITEM_HPP
#define PIXELVERSE_ITEM_HPP

#include <string>
#include "../utils/JsonHelper.hpp"

namespace pixelverse {
namespace inventory {

enum class ItemCategory {
    Weapon,
    Skin,
    Pet,
    PowerUp
};

enum class Rarity {
    Common,
    Rare,
    Epic,
    Legendary,
    Mythic
};

struct Item {
    int id;
    std::string name;
    ItemCategory category;
    Rarity rarity;
    int power;
    int priceCoins;
    int priceGems;
    bool equipped;

    Item(int itemId = 0, const std::string& n = "",
         ItemCategory cat = ItemCategory::Weapon,
         Rarity r = Rarity::Common, int p = 10,
         int coins = 100, int gems = 0, bool eq = false)
        : id(itemId), name(n), category(cat), rarity(r),
          power(p), priceCoins(coins), priceGems(gems), equipped(eq) {}

    static std::string rarityToString(Rarity r) {
        switch (r) {
            case Rarity::Common: return "Common";
            case Rarity::Rare: return "Rare";
            case Rarity::Epic: return "Epic";
            case Rarity::Legendary: return "Legendary";
            case Rarity::Mythic: return "Mythic";
        }
        return "Common";
    }

    static std::string categoryToString(ItemCategory cat) {
        switch (cat) {
            case ItemCategory::Weapon: return "Weapon";
            case ItemCategory::Skin: return "Skin";
            case ItemCategory::Pet: return "Pet";
            case ItemCategory::PowerUp: return "PowerUp";
        }
        return "Weapon";
    }

    utils::JsonValue toJson() const {
        utils::JsonValue json;
        json["id"] = id;
        json["name"] = name;
        json["category"] = categoryToString(category);
        json["rarity"] = rarityToString(rarity);
        json["power"] = power;
        json["priceCoins"] = priceCoins;
        json["priceGems"] = priceGems;
        json["equipped"] = equipped;
        return json;
    }
};

} // namespace inventory
} // namespace pixelverse

#endif // PIXELVERSE_ITEM_HPP

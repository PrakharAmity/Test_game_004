#include "InventoryManager.hpp"
#include <algorithm>

namespace pixelverse {
namespace inventory {

InventoryManager::InventoryManager() {
    // Default starter weapons, skins, pets
    items.push_back(Item(101, "Neon Plasma Katana", ItemCategory::Weapon, Rarity::Legendary, 140, 5000, 150, true));
    items.push_back(Item(102, "Cyber Void Bow", ItemCategory::Weapon, Rarity::Epic, 95, 3200, 80, false));
    items.push_back(Item(103, "Quantum Hyper-Revolver", ItemCategory::Weapon, Rarity::Rare, 70, 1800, 30, false));
    items.push_back(Item(201, "Chrono Valkyrie Outfit", ItemCategory::Skin, Rarity::Mythic, 0, 12000, 400, true));
    items.push_back(Item(202, "Glitch Shadow Robes", ItemCategory::Skin, Rarity::Epic, 0, 4500, 120, false));
    items.push_back(Item(301, "Mecha-Dragon Pet", ItemCategory::Pet, Rarity::Mythic, 50, 20000, 600, true));
    items.push_back(Item(401, "Overdrive XP Booster", ItemCategory::PowerUp, Rarity::Rare, 0, 800, 15, false));
}

void InventoryManager::addItem(const Item& item) {
    items.push_back(item);
}

void InventoryManager::sortItemsById() {
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.id < b.id;
    });
}

int InventoryManager::binarySearchById(int targetId) const {
    if (items.empty()) return -1;
    int low = 0;
    int high = static_cast<int>(items.size()) - 1;

    // Standard binary search condition (low <= high) with high = mid - 1
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (items[mid].id == targetId) {
            return mid;
        } else if (items[mid].id < targetId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

bool InventoryManager::removeSkin(int itemId) {
    if (items.empty()) return false;

    // Search item by ID using std::find_if, then erase the iterator
    auto it = std::find_if(items.begin(), items.end(), [itemId](const Item& item) {
        return item.id == itemId;
    });
    if (it != items.end()) {
        items.erase(it);
        return true;
    }
    return false;
}

bool InventoryManager::removeItemById(int itemId) {
    auto it = std::find_if(items.begin(), items.end(), [itemId](const Item& item) {
        return item.id == itemId;
    });
    if (it != items.end()) {
        items.erase(it);
        return true;
    }
    return false;
}

const Item* InventoryManager::findItemById(int itemId) const {
    for (const auto& item : items) {
        if (item.id == itemId) return &item;
    }
    return nullptr;
}

utils::JsonValue InventoryManager::toJson() const {
    utils::JsonValue json;
    json.type = utils::JsonValue::Type::Array;
    for (const auto& item : items) {
        json.arrayValue.push_back(item.toJson());
    }
    return json;
}

void InventoryManager::loadFromJson(const utils::JsonValue& json) {
    if (!json.isArray()) return;
    items.clear();
    for (const auto& val : json.arrayValue) {
        Item it;
        if (val.contains("id")) it.id = val["id"].asInt();
        if (val.contains("name")) it.name = val["name"].asString();
        if (val.contains("power")) it.power = val["power"].asInt();
        if (val.contains("priceCoins")) it.priceCoins = val["priceCoins"].asInt();
        if (val.contains("priceGems")) it.priceGems = val["priceGems"].asInt();
        if (val.contains("equipped")) it.equipped = val["equipped"].asBool();
        items.push_back(it);
    }
}

} // namespace inventory
} // namespace pixelverse

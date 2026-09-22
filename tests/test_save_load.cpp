#include "gtest/gtest.h"
#include "../src/player/Player.hpp"
#include "../src/inventory/InventoryManager.hpp"
#include "../src/utils/JsonHelper.hpp"
#include <cstdio>

using namespace pixelverse::player;
using namespace pixelverse::inventory;
using namespace pixelverse::utils;

TEST(SaveLoadTest, SaveLoad_SerializePlayerState) {
    Player original(42, "CyberTester", "avatar_cyber_42", 15, 2300, 99999, 1234, 4567, 88, 7);
    JsonValue json = original.toJson();

    Player loaded = Player::fromJson(json);
    EXPECT_EQ(loaded.id, 42);
    EXPECT_EQ(loaded.username, "CyberTester");
    EXPECT_EQ(loaded.level, 15);
    EXPECT_EQ(loaded.xp, 2300);
    EXPECT_EQ(loaded.coins, 99999);
    EXPECT_EQ(loaded.gems, 1234);
    EXPECT_EQ(loaded.trophies, 4567);
}

TEST(SaveLoadTest, SaveLoad_DeserializeCorruptSafe) {
    // Attempting to load from a non-existent file must not crash and should return empty/null JsonValue
    JsonValue val = JsonHelper::loadFromFile("save/non_existent_ghost_save_file.json");
    EXPECT_TRUE(val.isNull());
}

TEST(SaveLoadTest, SaveLoad_InventoryRoundTrip) {
    InventoryManager originalInv;
    originalInv.clear();
    originalInv.addItem(Item(99, "Hyper Glaive", ItemCategory::Weapon, Rarity::Mythic, 250, 8000, 200, true));
    originalInv.addItem(Item(88, "Chrono Phoenix", ItemCategory::Pet, Rarity::Legendary, 120, 15000, 500, false));

    JsonValue invJson = originalInv.toJson();
    InventoryManager loadedInv;
    loadedInv.loadFromJson(invJson);

    EXPECT_EQ(loadedInv.getItemCount(), 2u);
    const Item* item1 = loadedInv.findItemById(99);
    ASSERT_NE(item1, nullptr);
    EXPECT_EQ(item1->name, "Hyper Glaive");
    EXPECT_EQ(item1->power, 250);
    EXPECT_TRUE(item1->equipped);
}

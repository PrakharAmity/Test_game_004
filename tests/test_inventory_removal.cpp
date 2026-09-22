#include "gtest/gtest.h"
#include "../src/inventory/InventoryManager.hpp"

using namespace pixelverse::inventory;

TEST(InventoryRemovalTest, RemoveItem_ByIdMismatchIndex) {
    InventoryManager inv;
    inv.clear();
    // 3 items with distinct IDs that DO NOT equal their indices (0, 1, 2)
    inv.addItem(Item(501, "Neon Cloak", ItemCategory::Skin));
    inv.addItem(Item(502, "Cyber Armor", ItemCategory::Skin));
    inv.addItem(Item(503, "Shadow Veil", ItemCategory::Skin));

    // Request removal of item with ID 502
    // In Bug 6, removeSkin(502) evaluates 502 >= items.size() (3) and fails, or tries items.erase(items.begin() + 502)
    // Thus item 502 remains in the inventory!
    bool removed = inv.removeSkin(502);
    EXPECT_TRUE(removed);
    EXPECT_EQ(inv.getItemCount(), 2u);
    EXPECT_EQ(inv.findItemById(502), nullptr);
}

TEST(InventoryRemovalTest, RemoveItem_FirstItem) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(701, "FirstSkin", ItemCategory::Skin));
    inv.addItem(Item(702, "SecondSkin", ItemCategory::Skin));

    bool removed = inv.removeSkin(701);
    EXPECT_TRUE(removed);
    EXPECT_EQ(inv.getItemCount(), 1u);
    EXPECT_NE(inv.findItemById(702), nullptr);
    EXPECT_EQ(inv.findItemById(701), nullptr);
}

TEST(InventoryRemovalTest, RemoveItem_LastItem) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(801, "SkinA", ItemCategory::Skin));
    inv.addItem(Item(802, "SkinB", ItemCategory::Skin));

    bool removed = inv.removeSkin(802);
    EXPECT_TRUE(removed);
    EXPECT_EQ(inv.getItemCount(), 1u);
    EXPECT_EQ(inv.findItemById(802), nullptr);
}

TEST(InventoryRemovalTest, RemoveItem_NonExistentId) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(10, "SkinX", ItemCategory::Skin));

    bool removed = inv.removeSkin(999);
    EXPECT_FALSE(removed);
    EXPECT_EQ(inv.getItemCount(), 1u);
}

TEST(InventoryRemovalTest, RemoveItem_RepeatedRemovals) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(1001, "Skin1", ItemCategory::Skin));
    inv.addItem(Item(1002, "Skin2", ItemCategory::Skin));
    inv.addItem(Item(1003, "Skin3", ItemCategory::Skin));

    inv.removeSkin(1001);
    inv.removeSkin(1002);
    inv.removeSkin(1003);

    EXPECT_EQ(inv.getItemCount(), 0u);
}

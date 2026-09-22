#include "gtest/gtest.h"
#include "../src/inventory/InventoryManager.hpp"

using namespace pixelverse::inventory;

TEST(InventorySearchTest, BinarySearch_FindBoundaryItem) {
    InventoryManager inv;
    inv.clear();
    // Insert sorted items: IDs 10, 20, 30, 40, 50
    inv.addItem(Item(10, "Dagger", ItemCategory::Weapon, Rarity::Common));
    inv.addItem(Item(20, "Sword", ItemCategory::Weapon, Rarity::Common));
    inv.addItem(Item(30, "Axe", ItemCategory::Weapon, Rarity::Rare));
    inv.addItem(Item(40, "Staff", ItemCategory::Weapon, Rarity::Epic));
    inv.addItem(Item(50, "Orb", ItemCategory::Weapon, Rarity::Legendary));

    // Target is 50 (the upper boundary element at index 4)
    // With Bug 4, while (low < high) terminates when low == high == 4, returning -1!
    int idx = inv.binarySearchById(50);
    EXPECT_EQ(idx, 4);
}

TEST(InventorySearchTest, BinarySearch_FindMiddleItem) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(100, "Item1", ItemCategory::Weapon));
    inv.addItem(Item(200, "Item2", ItemCategory::Weapon));
    inv.addItem(Item(300, "Item3", ItemCategory::Weapon));

    int idx = inv.binarySearchById(200);
    EXPECT_EQ(idx, 1);
}

TEST(InventorySearchTest, BinarySearch_FindFirstItem) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(5, "ItemA", ItemCategory::Weapon));
    inv.addItem(Item(15, "ItemB", ItemCategory::Weapon));
    inv.addItem(Item(25, "ItemC", ItemCategory::Weapon));

    int idx = inv.binarySearchById(5);
    // Index 0
    EXPECT_EQ(idx, 0);
}

TEST(InventorySearchTest, BinarySearch_ItemNotFound) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(10, "Item1", ItemCategory::Weapon));
    inv.addItem(Item(20, "Item2", ItemCategory::Weapon));

    int idx = inv.binarySearchById(999);
    EXPECT_EQ(idx, -1);
}

TEST(InventorySearchTest, BinarySearch_SingleItemArray) {
    InventoryManager inv;
    inv.clear();
    inv.addItem(Item(77, "SoloItem", ItemCategory::Weapon));

    // For size 1: low=0, high=0.
    // In Bug 4, while (low < high) is false immediately (0 < 0 is false) and returns -1!
    int idx = inv.binarySearchById(77);
    EXPECT_EQ(idx, 0);
}

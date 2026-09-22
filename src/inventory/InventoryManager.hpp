#ifndef PIXELVERSE_INVENTORYMANAGER_HPP
#define PIXELVERSE_INVENTORYMANAGER_HPP

#include "Item.hpp"
#include <vector>

namespace pixelverse {
namespace inventory {

class InventoryManager {
private:
    std::vector<Item> items;

public:
    InventoryManager();

    void addItem(const Item& item);
    const std::vector<Item>& getItems() const { return items; }
    size_t getItemCount() const { return items.size(); }
    void clear() { items.clear(); }

    void sortItemsById();

    // BUG 4 (Intentional): Inventory Search Bug (DSA)
    // Concept: Binary Search on sorted std::vector
    // Symptom: Existing item sometimes not found (specifically at boundaries).
    // Returns index in items vector, or -1 if not found.
    int binarySearchById(int targetId) const;

    // BUG 6 (Intentional): Removing Skin Removes Wrong Item
    // Concept: Vector erase by index instead of ID.
    // Symptom: Wrong inventory item deleted when calling removeSkin.
    // Returns true if an item was removed.
    bool removeSkin(int itemId);

    bool removeItemById(int itemId);
    const Item* findItemById(int itemId) const;

    utils::JsonValue toJson() const;
    void loadFromJson(const utils::JsonValue& json);
};

} // namespace inventory
} // namespace pixelverse

#endif // PIXELVERSE_INVENTORYMANAGER_HPP

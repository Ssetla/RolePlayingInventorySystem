#include "Inventory.h"

unsigned int Inventory::getSize() const
{
    // get size of inventory
    return inventory.size();
}

void Inventory::forEach(const std::function<void(const Item&)>& accept) const
{
    //  Loop through inventory
    for (const Item* item : inventory) {
        if (item != nullptr) {
            accept(*item); 
        }
        else {
            // Throw logic_error
            throw std::logic_error("Item not found");
        }
    }
}

void Inventory::forEach(const std::function<void(const Item&)>& accept)
{
    // Loop through inventory
    for (const Item* item : inventory) {
        if (item != nullptr) {
            accept(*item);
        }
        else {
            // Throw logic_error
            throw std::logic_error("Item not found");
        }
    }
}

void Inventory::addItem(const Item& item)
{
    // Add new item to inventory
    const Item* newItem = item.clone();
    // Sort begining of inventory
    auto i{ inventory.begin() };
    // Sort largest to smallest
    while (i != inventory.end() && (item.getGoldValue() / item.getWeight()) < ((*i)->getGoldValue() / (*i)->getWeight())) {
        i++;
    }
    // Insert that item in proper spot in order to gold/weight ratio
    inventory.insert(i, newItem);
}

void Inventory::removeItem(const Item& item)
{
    // Remove item from inventory
    const Item* checkItem = &item;
    // Find function to remove
    std::list<const Item*>::iterator placement = find(inventory.begin(), inventory.end(), checkItem);
    if (placement == inventory.end()) {
        // Throw logic_error if item that is to be removed isn't there
        throw std::logic_error("Item not found in inventory");
    }
    else {
        const Item* newItem = &item; 
        // Otherwise remove item from list
        inventory.remove(newItem);
        delete &item;
    }
}

std::list<const Item*> Inventory::getInventory()
{

    return inventory;
}


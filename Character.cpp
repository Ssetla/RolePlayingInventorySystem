#include "Character.h"
#include <stdexcept>
#include <algorithm>
using namespace std;

const Collection<const Item>& Character::getInventory()
{
   // Returns inventory
   return inventory;
}

void Character::addItem(const Item& item)
{
    // Add item to inventory
    inventory.addItem(item);
}

void Character::dropItem(const Item& item)
{
    // Drop item from inventory
    inventory.removeItem(item);
}

double Character::getTotalWeight() const
{
    // Weight of items in inventory
    double sum = 0.0;
    // Shuffle through inventory
    inventory.forEach([&sum](const Item& current) {
        // Get sum of weight(s)
        sum += current.getWeight();
        });
    // Weight of weapon(s)
    for (const Armor* armor : equippedArmor) {
        if (armor != nullptr) {
            sum += armor->getWeight();
        }
    }
    // Weight of armor
    for (const Weapon* weapon : equippedWeapon) {
        if (weapon != nullptr) {
            sum += weapon->getWeight();
        }
    }
    // Return sum of weight
    return sum;
}

const Armor* Character::getEquippedArmor(unsigned int slotID) const
{
   // Find equipped armor
    for (unsigned int i = 0; i < equippedArmor.size(); i++) {
        // Check if slot is within range
        if (slotID >= 6 || slotID < 0) {
            throw out_of_range("invalid SlotID");
        }
        // Check if armor isn't null and in range
        if (equippedArmor.at(i) != nullptr && slotID >= 0) {
            // Return armor at slotID
            if (equippedArmor.at(i)->getSlotID() == slotID) {
                return equippedArmor.at(i);
            }
        }
    }
    // Otherwise return nullptr
    return nullptr;
}

int Character::getTotalArmorRating() const
{
    // Initial sum
    int sum = 0;
    // Loop through equipped armor
    for (unsigned int i = 0; i < equippedArmor.size(); i++) {
        // Check if empty
        if (equippedArmor.at(i) != nullptr) {
            // Get armor rating
            sum += equippedArmor.at(i)->getRating();
        }
    }
    // Otherwise return sum
    return sum;
    
}

void Character::equipArmor(const Armor& armor)
{
    // Initiate to armor's slot id
    unsigned int slotID{ armor.getSlotID() };
    // Check count
    int check{ 0 };
    // Loop through the inventory
    for (const Item* item : inventory.getInventory())
    {
        // Search through armor list for item
        if (dynamic_cast<const Armor*>(item) != &armor)
        {
            // Increment check
            check++;
        }
    }
    // Check if inventory size is the same for checking item list #
    if (check >= inventory.getSize())
    {
        // Otherwise throw logic_error
        throw std::logic_error("Weapon could not be found in inventory.");
    }
    // Check if there is armor in the slot
    if (equippedArmor.at(slotID) != nullptr) {
        // Unequip armor
        unequipArmor(slotID);
    }
    // Equip armor from inventory
    equippedArmor.at(slotID) = armor.clone();
    // Delete recently equipped armor from inventory (clone version now equipped)
    inventory.removeItem(armor);
    
}

void Character::unequipArmor(unsigned int slotID)
{
    // Add item to armor slot from inventory
    if (slotID >= 6 || slotID < 0) {
        // Out of range exception if outside slot id #
        throw std::out_of_range("Invalid slotID");
    }
    // Check if armor at slot id
    else if (equippedArmor.at(slotID) != nullptr) {
        // Add armor to delete list
        const Item* toDelete{ equippedArmor.at(slotID) };
        // Add armor back to inventory
        addItem(*toDelete);
        // Check if there isn't anything equipped
        equippedArmor.at(slotID) = nullptr;
        // Clear delete list
        delete toDelete;
    }
}

const Weapon* Character::getEquippedWeapon() const
{
    // Return current equipped weapon
    if (equippedWeapon.at(0) != nullptr) {
        return equippedWeapon.at(0);
    }
    // Otherwise return nullptr
    return nullptr;
}

void Character::equipWeapon(const Weapon& weapon)
{
    int check{ 0 };
    // Loop through inventory
    for (const Item* item : inventory.getInventory())
    {
        // Find what items are not weapons
        if (dynamic_cast<const Weapon*>(item) != &weapon)
        {
            // Increment check
            check++;
        }
    }
    // Compare check with inventory size
    if (check >= inventory.getSize())
    {
        // Throw logic_error if no weapon found in inventory
        throw std::logic_error("Weapon could not be found in inventory.");
    }
    // Also check if there is a current weapon equipped
    if (equippedWeapon.at(0) != nullptr) {
        // Unequip the weapon
        unequipWeapon();
    }
    // Equip weapon to weapon array
    const Weapon* newWeapon = weapon.clone();
    // Remove equipped weapon from inventory
    inventory.removeItem(weapon);
    equippedWeapon.at(0) = newWeapon;
    
}

void Character::unequipWeapon()
{
    // Unequip weapon. Check if there's a weapon equipped   
    if (equippedWeapon.at(0) != nullptr) {
        // Add weapon to delete list
        const Weapon* toDelete{ equippedWeapon.at(0) };
        // Add weapon back to inventory
        inventory.addItem(*equippedWeapon.at(0));
        // Check if there is a weapon equipped
        equippedWeapon.at(0) = nullptr;
        // Clear delete list
        delete toDelete;
    }
}

void Character::optimizeInventory(double maximumWeight)
{
    // Out of range exception
    if (maximumWeight < 0) {
        throw out_of_range("Maximum weight is 0");
    }
    // Delete weight til below totalWeight
    if (getTotalWeight() > maximumWeight ) {
        double totalWeight = getTotalWeight();
        // Create a list of all items
        std::list<const Item*> allItems;
        // go through inventory
        for (const Item* item : inventory.getInventory()) {
            if (item != nullptr) {
                // Add to back of list
                allItems.push_back(item);
            }
        }
        // Lambda
        allItems.sort( [](const Item* a, const Item* b)->bool {return a->getGoldValue() / a->getWeight() < b->getGoldValue() / b->getWeight(); });
        // Go through the inventory and remove items over the weight limit
        auto i{ allItems.begin() };
        // Check to make sure weight isn't larger than maximum weight
        while (i != allItems.end() && totalWeight > maximumWeight) {
            totalWeight -= (*i)->getWeight();
            // Remove item
            inventory.removeItem(**i);
            i++;
        }
    }
}

void Character::optimizeEquipment()
{
    // Get best valued armor rating and highest damage sword
    for (const Item* item : inventory.getInventory()) {
        // Dynamic_cast armor array and look for nullptr
        if (dynamic_cast<const Armor*>(item) != nullptr) {
            const Armor* armorPtr = dynamic_cast<const Armor*>(item);
            const Armor* equippedPtr = getEquippedArmor(armorPtr->getSlotID());
            if (equippedPtr == nullptr || armorPtr->getRating() > equippedPtr->getRating()) {
                /*unequipArmor(armorPtr->getSlotID());*/
                equipArmor(*armorPtr);
            }
        }
        // Dynamic_cast weapon slot and look for nullptr
        else if (dynamic_cast<const Weapon*>(item) != nullptr) {
            const Weapon* weaponPtr = dynamic_cast<const Weapon*>(item);
            const Weapon* equippedPtr = getEquippedWeapon();
            if (equippedPtr == nullptr || weaponPtr->getDamage() > equippedPtr->getDamage()) {
                /*unequipWeapon();*/
                equipWeapon(*weaponPtr);
            }
        }
    }
}

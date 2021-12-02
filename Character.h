#pragma once
#include "Item.h"
#include "Armor.h"
#include "Weapon.h"
#include "Collection.h"
#include "Inventory.h"
#include <array>

// A class for keeping track of the equipped items and inventory of a character in a role-playing game.
class Character
{
public:
    // Default constructor
    Character() = default;

    // Copy constructor deleted for simplicity; shouldn't be needed for any of the tests.
    Character(const Character& character) = delete;

    // Copy assignment deleted for simplicity; shouldn't be needed for any of the tests.
    Character& operator = (const Character& character) = delete;

    // Returns an implementation of the Collection interface that provides read-only access to
    // the items in the character’s inventory (that is, items that are not equipped as armor or a
    // weapon).  The items should be sorted in descending value-to-weight ratio.
    const Collection<const Item>& getInventory();

    // Adds a copy of the specified item to the inventory.  In other words, the Item passed in is
    // the “pattern” for a new item that should be created and added to the inventory.
    void addItem(const Item& item);

    // Searches for and removes the specified item from the inventory.  
    // A logic_error should be thrown if the item cannot be found in the inventory.
    void dropItem(const Item& item);
    
    // Returns the total “weight” of all items, whether equipped or in the inventory.
    double getTotalWeight() const;

    // Gets the armor piece equipped in a particular slot, or nullptr if no armor is equipped in 
    // that slot.  An out_of_range exception should be thrown if slotID is not 0, 1, 2, 3, 4, or 5.
    const Armor* getEquippedArmor(unsigned int slotID) const;

    // Gets the sum of the armor rating values over all equipped armor pieces.  
    // Armor in the inventory doesn’t count towards this sum.
    int getTotalArmorRating() const;

    // Searches the inventory for a specific armor piece, removes it from the inventory, and equips
    // it in its specified armor slot.  If there is already a piece of armor in the required slot, 
    // the old piece of armor is automatically unequipped and returned to the inventory.  
    // This function must ensure that the specified piece of armor is already in the inventory.  
    // A logic_error should be thrown if the piece of armor cannot be found in the inventory.
    void equipArmor(const Armor& armor);

    // Unequips the piece of armor in the specified slot and returns it to the inventory.  
    // If no armor is equipped in that slot, this function has no effect.  
    // An out_of_range exception should be thrown if slotID is not 0, 1, 2, 3, 4, or 5.
    void unequipArmor(unsigned int slotID);

    // Gets the currently equipped weapon, or nullptr if no weapon is equipped.
    const Weapon* getEquippedWeapon() const;

    // Searches the inventory for a specific weapon, removes it from the inventory, and equips it.  
    // If there is already an equipped weapon, the old weapon is automatically unequipped and 
    // returned to the inventory.  This function must ensure that the specified weapon is already 
    // in the inventory.  A logic_error should be thrown if the weapon cannot be found.
    void equipWeapon(const Weapon& weapon);

    // Unequips the currently equipped weapon and returns it to the inventory.  
    // If no weapon is equipped, this function has no effect.
    void unequipWeapon();

    // “Optimizes” the inventory by dropping items until the total character weight (including both
    // the inventory and equipped weapons and armor) is less than the specified maximum weight.  
    // The items to drop should be chosen by starting with the lowest value to weight ratio and
    // working up.  Despite contributing towards the weight total, equipped weapons and armor should
    // never be dropped regardless of their value to weight ratio.  If the total weight of equipped 
    // weapons and armor alone is greater than or equal to the specified maximum weight, then all 
    // non-weightless items in the inventory should be dropped.  
    // An out_of_range exception should be thrown if maximumWeight is less than zero.
    void optimizeInventory(double maximumWeight);

    // “Optimizes” the currently equipped weapons and armor by equipping the weapon with the highest
    // damage rating and the armor piece with the highest armor rating for each armor slot.
    // Any previously equipped weapon or armor that is no longer optimal is returned to the inventory.
    void optimizeEquipment();

private:
    // The instance of the inventory class, which will hold items not currently equipped.
    Inventory inventory {};

    // TODO: Add your own private variables here:
    // Eqiupped armor array
    std::array<const Armor*, 6>equippedArmor{};
    // Equipped weapon array
    std::array<const Weapon*, 1>equippedWeapon{};
};
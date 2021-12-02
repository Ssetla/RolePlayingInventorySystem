#pragma once
#include "Collection.h"
#include "Item.h"
#include <list>


// An implementation of Collection for providing readonly access to the items in a character's inventory.
class Inventory : public Collection<const Item>
{
public:
    // Gets the number of elements in the collection.
    virtual unsigned int getSize() const;

    // Performs the specified accept() function on each element in the collection (read-only).
    virtual void forEach(const std::function<void(const Item&)>& accept) const;

    // Performs the specified accept() function on each element in the collection, 
    // potentially making changes to elements as they’re visited.
    virtual void forEach(const std::function<void(const Item&)>& accept);

    // TODO: Add other functions you need here.
    // Add item to inventory
    void addItem(const Item& item);

    // Remove item from inventory
    void removeItem(const Item& item);

    // Get inventory function
    std::list<const Item*> getInventory();

private:
    // TODO: Add private variables and subroutines here.
    // List container for invent.
    std::list<const Item*> inventory{};
    // List iterator for invent.
    std::list<const Item*>::iterator position{};
};

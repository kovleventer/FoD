#pragma once

#include "unit.h"
#include "item.h"

//Handles unit type and item type relations
class UnitInventoryHandler {
public:
	//If a specific type of item can added to a specific type of unit
	static bool matches(UnitType unitType, ItemType itemType);
	
	//If a unit has already a type of that item
	static bool hasType(Unit* unit, ItemType itemType);
};

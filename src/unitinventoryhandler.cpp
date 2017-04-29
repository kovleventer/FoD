#include "unitinventoryhandler.h"

/*!
 * @author kovlev
 */

bool UnitInventoryHandler::matches(UnitType unitType, ItemType itemType) {
	switch (unitType) {
		case UnitType::FIGHTER:
			return (itemType == ItemType::MELEE ||
				itemType == ItemType::SHIELD ||
				itemType == ItemType::ARMOR ||
				itemType == ItemType::JEWELLERY);
			break;
		case UnitType::ARCHER:
			return (itemType == ItemType::BOW ||
				itemType == ItemType::ARROW ||
				itemType == ItemType::ARMOR ||
				itemType == ItemType::JEWELLERY);
			break;
		case UnitType::MAGE:
			return (itemType == ItemType::STAFF ||
				itemType == ItemType::MAGICSHIELD ||
				itemType == ItemType::CLOAK ||
				itemType == ItemType::JEWELLERY);
			break;
		case UnitType::OTHER:
			return false;
			break;
	}
	return false;
}

bool UnitInventoryHandler::hasType(Unit* unit, ItemType itemType) {
	for (int i = 0; i < unit->getUnitInventorySize(); i++) {
		if (unit->getItem(i) != NULL) {
			if (unit->getItem(i)->getItemType() == itemType) {
				return true;
			}
		}
	}
	return false;
}

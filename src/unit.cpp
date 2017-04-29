#include "unit.h"

#include "global.h"
#include "unitinventoryhandler.h"

Unit::Unit(std::string n, UnitType uT) {
	name = n;
	unitType = uT;
	texture = Global::resourceHandler->unitTextures[name];
	
	unitInventorySize = 4;
	
	//Setting the unit's inventory
	items = new Item*[unitInventorySize];
	for (int i = 0; i < unitInventorySize; i++) {
		items[i] = NULL;
	}
}

Unit::~Unit() {
	delete[] items;
}

void Unit::render(int x, int y, int w, int h) {
	SDL_Rect destinationRect = {x, y, w, h};
	render(destinationRect);
}

void Unit::render(SDL_Rect destinationRect) {
	SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
	//Unit wounded inicator as red rectangle
	SDL_SetRenderDrawColor(Global::renderer, 0x74, 0x00, 0x00, 0xAF);
	destinationRect.h = (int)((double)(statsWithItems["life"] - statsWithItems["currentLife"]) / statsWithItems["life"] * destinationRect.h);
	SDL_RenderFillRect(Global::renderer, &destinationRect);
}

std::string Unit::getName() {
	return name;
}

int Unit::getUnitInventorySize() {
	return unitInventorySize;
}

bool Unit::addItem(Item* itemToAdd) {
	if (UnitInventoryHandler::matches(unitType, itemToAdd->getItemType()) && !UnitInventoryHandler::hasType(this, itemToAdd->getItemType())) {
		for (int i = 0; i < unitInventorySize; i++) {
			if (items[i] == NULL) {
				items[i] = itemToAdd;
				return true;
			}
		}
	}
	return false;
}

Item* Unit::removeItem(int position) {
	Item* itemToRemove = items[position];
	items[position] = NULL;
	return itemToRemove;
}

Item* Unit::getItem(int position) {
	return items[position];
}

void Unit::recalculateInventory() {
	statsWithItems = stats;
	for (int i = 0; i < unitInventorySize; i++) {
		Item* tempItem = getItem(i);
		if (tempItem != NULL) {
			for(std::map<std::string, int>::const_iterator it = tempItem->stats.begin(); it != tempItem->stats.end(); ++it) {
				statsWithItems[it->first] += it->second;
				if (it->first == "life") {
					statsWithItems["currentLife"] += it->second;
				}
			}
		}
	}
}

bool UnitSpeedComparator::operator()(Unit* a, Unit* b) {
	return (a->statsWithItems["speed"] < b->statsWithItems["speed"]);
}

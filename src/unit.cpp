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
	
	dead = false;
	if (deadTexture == NULL) {
		Unit::deadTexture = Global::resourceHandler->unitTextures["_Dead"];
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
	if (dead) {
		SDL_RenderCopy(Global::renderer, Unit::deadTexture, NULL, &destinationRect);
	} else {
		SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
		//Unit wounded inicator as red rectangle
		SDL_SetRenderDrawColor(Global::renderer, 0x74, 0x00, 0x00, 0xAF);
		destinationRect.h = (int)((double)(statsWithItems["life"] - statsWithItems["currentLife"]) / statsWithItems["life"] * destinationRect.h);
		SDL_RenderFillRect(Global::renderer, &destinationRect);
	}
}

std::string Unit::getName() {
	return name;
}

int Unit::getUnitInventorySize() {
	return unitInventorySize;
}

bool Unit::getTeamOne() {
	return isTeamOne;
}

Point Unit::getPosition() {
	return position;
}

bool Unit::isDead() {
	return dead;
}

UnitType Unit::getUnitType() {
	return unitType;
}

int Unit::getLevel() {
	return level;
}

void Unit::setTeamOne(bool newTeamOne) {
	isTeamOne = newTeamOne;
}

void Unit::setPositionIndicator(Point newPos) {
	position = newPos;
}

void Unit::setPositionIndicator(int newX, int newY) {
	setPositionIndicator(Point(newX, newY));
}

void Unit::setLevel(int newLevel) {
	level = newLevel;
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

void Unit::kill() {
	dead = true;
}

SDL_Texture* Unit::deadTexture = NULL;

bool UnitSpeedComparator::operator()(Unit* a, Unit* b) {
	return (a->statsWithItems["speed"] < b->statsWithItems["speed"]);
}

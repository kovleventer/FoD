#include "unit.h"

#include "global.h"
#include "unitinventoryhandler.h"

/*!
 * @author kovlev
 */

Unit::Unit(std::string n, UnitType uT) {
	name = n;
	unitType = uT;
	texture = Global::resourceHandler->getATexture(TT::UNIT, name);
	
	unitInventorySize = 4;
	
	//Setting the unit's inventory
	items = new Item*[unitInventorySize];
	for (int i = 0; i < unitInventorySize; i++) {
		items[i] = NULL;
	}
	
	dead = false;
	if (deadTexture == NULL) {
		Unit::deadTexture = Global::resourceHandler->getATexture(TT::UNIT, "_Dead");
	}
	
	clearTempXP();
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
		Unit::deadTexture->render(destinationRect);
	} else {
		texture->render(destinationRect);
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

int Unit::getTemporaryXP() {
	return temporaryExperienceHolder;
}

bool Unit::isMelee() {
	return unitType == UnitType::FIGHTER || unitType == UnitType::OTHER;
}

bool Unit::isRanged() {
	return !isMelee();
}

void Unit::setTeamOne(bool newTeamOne) {
	isTeamOne = newTeamOne;
}

void Unit::setPositionIndicator(Point newPos) {
	//Weird NULL checking
	//It would cause an error otherwise
	if (this != NULL) {
		position = newPos;
	}
}

void Unit::setPositionIndicator(int newX, int newY) {
	setPositionIndicator(Point(newX, newY));
}

void Unit::setLevel(int newLevel) {
	level = newLevel;
}

void Unit::addTempXP(int xpToAdd) {
	temporaryExperienceHolder += xpToAdd;
}

void Unit::clearTempXP() {
	temporaryExperienceHolder = 0;
}

bool Unit::addItem(Item* itemToAdd) {
	//NULL checking
	if (itemToAdd == NULL) return false;
	
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

int Unit::getAttack() {
	int attackValue = 0;
	switch(getUnitType()) {
		case UnitType::FIGHTER:
		case UnitType::OTHER:
			attackValue = statsWithItems["meleeDamage"];
			break;
		case UnitType::ARCHER:
			attackValue = statsWithItems["shootingDamage"];
			break;
		case UnitType::MAGE:
			attackValue = statsWithItems["magicDamage"];
			break;
	}
	return attackValue;
}

int Unit::getDefense(Unit* attackingUnit) {
	int defenseValue = 0;
	switch(attackingUnit->getUnitType()) {
		case UnitType::FIGHTER:
		case UnitType::OTHER:
		case UnitType::ARCHER:
			defenseValue = statsWithItems["physicalDefense"];
			break;
		case UnitType::MAGE:
			defenseValue = statsWithItems["magicDefense"];
			break;
	}
	return defenseValue;
}

void Unit::recalculateInventory() {
	//Oh fuckmylife, i need to use maps for storing stats because its way too elegant here to give up
	statsWithItems = stats;
	for (int i = 0; i < unitInventorySize; i++) {
		Item* tempItem = getItem(i);
		if (tempItem != NULL) {
			for(std::map<std::string, int>::const_iterator it = tempItem->stats.begin(); it != tempItem->stats.end(); ++it) {
				statsWithItems[it->first] += it->second;
				if (it->first == "life" && statsWithItems["currentLife"] != 0) {
					statsWithItems["currentLife"] += it->second;
				}
				if (it->first == "numberOfActions") {
					statsWithItems["currentNumberOfActions"] += it->second;
				}
			}
		}
	}
}

void Unit::kill() {
	dead = true;
	clearTempXP();
}

bool Unit::finalizeExperience() {
	stats["currentExperience"] += temporaryExperienceHolder;
	statsWithItems["currentExperience"] += temporaryExperienceHolder;
	clearTempXP();
	
	return stats["experience"] <= stats["currentExperience"];
}

ATexture* Unit::deadTexture = NULL;

bool UnitSpeedComparator::operator()(Unit* a, Unit* b) {
	return (a->statsWithItems["speed"] < b->statsWithItems["speed"]);
}

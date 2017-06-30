#include "unit.h"

#include "../core/global.h"
#include "unitinventoryhandler.h"

/*!
 * @author kovlev
 */

Unit::Unit(std::string n, UnitType uT) {
	name = n;
	unitType = uT;
	texture = Global::resourceHandler->getATexture(TT::UNIT, name);
	
	unitInventorySize = 4;
	
	miniStatsFontSize = 17;
	
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
	
	//Most effective solution
	if (this == Global::player->getArmy()->getUnitInfo()->getUnit()) {
		Global::player->getArmy()->getUnitInfo()->setUnit(NULL);
	}
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

void Unit::renderMiniStats(SDL_Rect destinationRect) {
	Dimension d;
	SDL_Rect tempDestRect;
	
	ATexture* attackText = Global::resourceHandler->getTextTexture("A: " + std::to_string(getAttack()), Global::resourceHandler->getColor("whole-header"), miniStatsFontSize);
	d = attackText->getDimensions();
	tempDestRect.x = destinationRect.x;
	tempDestRect.y = destinationRect.y;
	tempDestRect.w = d.W();
	tempDestRect.h = d.H();
	attackText->render(tempDestRect);
	
	ATexture* defenseText = Global::resourceHandler->getTextTexture(
		"D: " + std::to_string(statsWithItems["physicalDefense"]) + "/" +  std::to_string(statsWithItems["magicDefense"]),
													Global::resourceHandler->getColor("whole-header"), miniStatsFontSize);
	d = defenseText->getDimensions();
	tempDestRect.x = destinationRect.x + destinationRect.w - d.W();
	tempDestRect.y = destinationRect.y;
	tempDestRect.w = d.W();
	tempDestRect.h = d.H();
	defenseText->render(tempDestRect);
	
	ATexture* lifeText = Global::resourceHandler->getTextTexture(
		"L: " + std::to_string(statsWithItems["currentLife"]) + "/" + std::to_string(statsWithItems["life"]),
													Global::resourceHandler->getColor("whole-header"), miniStatsFontSize);
	d = lifeText->getDimensions();
	tempDestRect.x = destinationRect.x;
	tempDestRect.y = destinationRect.y + destinationRect.h - d.H();
	tempDestRect.w = d.W();
	tempDestRect.h = d.H();
	lifeText->render(tempDestRect);
	
	ATexture* noaText = Global::resourceHandler->getTextTexture(
		"N: " + std::to_string(statsWithItems["currentNumberOfActions"]) /*+ "/" + std::to_string(statsWithItems["numberOfActions"])*/,
													Global::resourceHandler->getColor("whole-header"), miniStatsFontSize);
	d = noaText->getDimensions();
	tempDestRect.x = destinationRect.x + destinationRect.w - d.W();
	tempDestRect.y = destinationRect.y + destinationRect.h - d.H();
	tempDestRect.w = d.W();
	tempDestRect.h = d.H();
	noaText->render(tempDestRect);
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
	//I need to use maps for storing stats because its way too elegant here to give up
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
	if (statsWithItems["currentLife"] <= 0) {
		//Maybe not the best solution
		kill();
		statsWithItems["currentLife"] = 0;
	}
}

int Unit::getUnitValue(bool withItems) {
	int meleeMultipliers[] = {2, 2, 4, 2};
	int rangedMultipliers[] = {1, 3, 2, 4};
	if (withItems) {
		int multiplicands[] = {statsWithItems["currentLife"],
									getAttack() * statsWithItems["numberOfActions"],
									(statsWithItems["physicalDefense"] + statsWithItems["magicDefense"]) / 2,
									statsWithItems["speed"]};
		if (isMelee()) {
			return std::inner_product(std::begin(meleeMultipliers), std::end(meleeMultipliers), std::begin(multiplicands), 0);
		} else {
			return std::inner_product(std::begin(rangedMultipliers), std::end(rangedMultipliers), std::begin(multiplicands), 0);
		}
	} else {
		int multiplicands[] = {stats["currentLife"],
								getAttack() * stats["numberOfActions"],
								(stats["physicalDefense"] + stats["magicDefense"]) / 2,
								stats["speed"]};
		if (isMelee()) {
			return std::inner_product(std::begin(meleeMultipliers), std::end(meleeMultipliers), std::begin(multiplicands), 0);
		} else {
			return std::inner_product(std::begin(rangedMultipliers), std::end(rangedMultipliers), std::begin(multiplicands), 0);
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

bool UnitValueComparatorWItems::operator()(Unit* a, Unit* b) {
	return (a->getUnitValue(true) < b->getUnitValue(true));
}

bool UnitValueComparatorNoItems::operator()(Unit* a, Unit* b) {
	return (a->getUnitValue(false) < b->getUnitValue(false));
}

#include "character.h"

#include "../core/global.h"
#include "worldobject.h"

/*!
 * @author kovlev
 */

Character::Character(Point pos) : MapEntity(pos) {
	gold = 0;
}

Character::Character(int x, int y) : Character(Point(x, y)) {}

Character::~Character() {}

PointD Character::getProgressVector() {
	return progressVector;
}

double Character::getSpeed() {
	return speed;
}

Inventory* Character::getInventory() {
	return inventory;
}

Army* Character::getArmy() {
	return army;
}

std::string Character::getName() {
	return name;
}

int Character::getGold() {
	return gold;
}

void Character::setProgressVector(PointD newProgressVector) {
	progressVector = newProgressVector;
}

void Character::setSpeed(double newSpeed) {
	speed = newSpeed;
}

void Character::setName(std::string newName) {
	name = newName;
}

void Character::giveGold(int goldToGive) {
	//NOTE no negative value checking
	gold += goldToGive;
}

void Character::takeGold(int goldToTake) {
	//NOTE no negative value checking
	gold -= goldToTake;
}

void Character::addOwned(InteractiveWorldObject* interactiveToAdd) {
	ownedBuildings.push_back(interactiveToAdd);
}

void Character::removeOwned(InteractiveWorldObject* interactiveToRemove) {
	ownedBuildings.erase(std::remove(ownedBuildings.begin(), ownedBuildings.end(), interactiveToRemove), ownedBuildings.end());
}

Character* Character::characterPlaceholderTakeable = NULL;

void Character::calcRotation(Point pRot) {
	if (pRot == Point(1, -1)) {
		//Up right
		texture->setRotation(RotationType::UPRIGHT);
	} else if (pRot == Point(1, 0)) {
		//Right
		texture->setRotation(RotationType::RIGHT);
	} else if (pRot == Point(1, 1)) {
		//Down right
		texture->setRotation(RotationType::DOWNRIGHT);
	} else if (pRot == Point(0, 1)) {
		//Down
		texture->setRotation(RotationType::DOWN);
	} else if (pRot == Point(-1, 1)) {
		//Down left
		texture->setRotation(RotationType::DOWNLEFT);
	} else if (pRot == Point(-1, 0)) {
		//Left
		texture->setRotation(RotationType::LEFT);
	} else if (pRot == Point(-1, -1)) {
		//Up left
		texture->setRotation(RotationType::UPLEFT);
	} else if (pRot == Point(0, -1)) {
		//Up
		texture->setRotation(RotationType::UP);
	} else {
		std::clog << "Warning: Rotation calculating bug" << std::endl;
	}
}

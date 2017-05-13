#include "abstractunit.h"

/*!
 * @author kovlev
 */

AbstractUnit::AbstractUnit() {}

std::string AbstractUnit::getName() {
	return name;
}

UnitType AbstractUnit::getUnitType() {
	return unitType;
}

int AbstractUnit::getLife() {
	return life;
}

int AbstractUnit::getMeleeDamage() {
	return meleeDamage;
}

int AbstractUnit::getShootingDamage() {
	return shootingDamage;
}

int AbstractUnit::getMagicDamage() {
	return magicDamage;
}

int AbstractUnit::getPhysicalDefense() {
	return physicalDefense;
}

int AbstractUnit::getMagicDefense() {
	return magicDefense;
}

int AbstractUnit::getSpeed() {
	return speed;
}

int AbstractUnit::getNumberOfActions() {
	return numberOfActions;
}

int AbstractUnit::getExperience() {
	return experience;
}

int AbstractUnit::getPrice() {
	return price;
}

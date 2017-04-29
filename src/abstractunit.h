#pragma once

#include <string>

//Required because of friend relations
class AbstractUnit;

#include "unithandler.h"
#include "unit.h"

class AbstractUnit {
	friend class UnitHandler;
public:
	AbstractUnit();
	
	//Getters
	std::string getName();
	UnitType getUnitType();
	int getLife();
	int getMeleeDamage();
	int getShootingDamage();
	int getMagicDamage();
	int getPhysicalDefense();
	int getMagicDefense();
	int getSpeed();
	int getNumberOfActions();
	int getExperience();
	//No setters, stats are setted via friend classes
private:
	std::string name;
	UnitType unitType;
	int life;
	int meleeDamage;
	int shootingDamage;
	int magicDamage;
	int physicalDefense;
	int magicDefense;
	int speed;
	int numberOfActions;
	int experience;
};

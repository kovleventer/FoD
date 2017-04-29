#include "unithandler.h"

UnitHandler::UnitHandler() {
	basePath = "data/unit/";
	levelMultipliers = 1.1;
	levelMultipliersForEXP = 1.2;
	levelMultipliersForNOA = 1.01;
	
	loadUnitData();
}

UnitHandler::~UnitHandler() {
	for(std::map<std::string, AbstractUnit*>::const_iterator it = baseUnits.begin(); it != baseUnits.end(); ++it) {
		delete it->second;
	}
}

Unit* UnitHandler::getUnit(std::string name, int level) {
	
	if (baseUnits.find(name) == baseUnits.end()) {
		std::cout << baseUnits["axeguy"] << std::endl;
		return NULL;
	} else {
		//NOTE ugly
		AbstractUnit* unitTemplate = baseUnits[name];
		Unit* newUnit = new Unit(unitTemplate->getName());
		newUnit->stats["life"] = (int)((double)unitTemplate->getLife() * pow(levelMultipliers, level - 1));
		newUnit->stats["currentLife"] = newUnit->stats["life"];
		newUnit->stats["meleeDamage"] = (int)((double)unitTemplate->getMeleeDamage() * pow(levelMultipliers, level - 1));
		newUnit->stats["shootingDamage"] = (int)((double)unitTemplate->getShootingDamage() * pow(levelMultipliers, level - 1));
		newUnit->stats["magicDamage"] = (int)((double)unitTemplate->getMagicDamage() * pow(levelMultipliers, level - 1));
		newUnit->stats["physicalDefense"] = (int)((double)unitTemplate->getPhysicalDefense() * pow(levelMultipliers, level - 1));
		newUnit->stats["magicDefense"] = (int)((double)unitTemplate->getMagicDefense() * pow(levelMultipliers, level - 1));
		newUnit->stats["speed"] = (int)((double)unitTemplate->getSpeed() * pow(levelMultipliers, level - 1));
		newUnit->stats["numberOfActions"] = (int)((double)unitTemplate->getNumberOfActions() * pow(levelMultipliersForNOA, level - 1));
		newUnit->stats["experience"] = (int)((double)unitTemplate->getExperience() * pow(levelMultipliersForEXP, level - 1));
		newUnit->stats["currentExperience"] = 0;
		
		newUnit->statsWithItems = newUnit->stats;
		
		//Incrementing counter
		numberOfCreatedUnits++;
		
		return newUnit;
	}
}

void UnitHandler::loadUnitData() {
	std::vector<std::string> unitList = {"axeguy"};
	
	//NOTE uses file IO
	std::fstream file;
	
	//File pattern:
	//Name
	//life;
	//meleeDamage;
	//shootingDamage;
	//magicDamage;
	//physicalDefense;
	//magicDefense;
	//speed;
	//numberOfActions;
	//experience;
	for (unsigned int i = 0; i < unitList.size(); i++) {
		//Loading the units into level 1 unittemplates
		file.open(basePath + unitList[i] + ".data", std::ios::in);
		
		AbstractUnit* currentUnit = new AbstractUnit();
		std::string name;
		file >> name;
		currentUnit->name = name;
		//Can be easily done due to friend relations
		file >> currentUnit->life;
		file >> currentUnit->meleeDamage;
		file >> currentUnit->shootingDamage;
		file >> currentUnit->magicDamage;
		file >> currentUnit->physicalDefense;
		file >> currentUnit->magicDefense;
		file >> currentUnit->speed;
		file >> currentUnit->numberOfActions;
		file >> currentUnit->experience;
		
		baseUnits[name] = currentUnit;
		
		file.close();
	}
	
}

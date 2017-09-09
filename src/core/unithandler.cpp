#include "unithandler.h"

#include "../player/unit.h"

/*!
 * @author kovlev
 */

UnitHandler::UnitHandler() {
	basePath = "data/unit/";
	levelMultipliers = 1.1;
	levelMultipliersForEXP = 1.2;
	levelMultipliersForNOA = 1.01;
	
	translaterInit();
	loadUnitData();
}

UnitHandler::~UnitHandler() {
	for(std::map<std::string, AbstractUnit*>::const_iterator it = baseUnits.begin(); it != baseUnits.end(); ++it) {
		delete it->second;
	}
}

Unit* UnitHandler::getUnit(std::string name, int level) {
	
	if (level <= 0) {
		//Invalid level
		std::clog << "Warning! \"" << name << "\" has invalid level of " << level << std::endl;
	}
	
	if (baseUnits.find(name) == baseUnits.end()) {
		std::clog << "Error! Unit with name \"" << name << "\" does not exist." << std::endl;
		return NULL;
	} else {
		//NOTE ugly
		AbstractUnit* unitTemplate = baseUnits[name];
		Unit* newUnit = new Unit(unitTemplate->getName(), unitTemplate->getUnitType());
		newUnit->stats["life"] = (int)((double)unitTemplate->getLife() * pow(levelMultipliers, level - 1));
		newUnit->stats["currentLife"] = newUnit->stats["life"];
		newUnit->stats["meleeDamage"] = (int)((double)unitTemplate->getMeleeDamage() * pow(levelMultipliers, level - 1));
		newUnit->stats["shootingDamage"] = (int)((double)unitTemplate->getShootingDamage() * pow(levelMultipliers, level - 1));
		newUnit->stats["magicDamage"] = (int)((double)unitTemplate->getMagicDamage() * pow(levelMultipliers, level - 1));
		newUnit->stats["physicalDefense"] = (int)((double)unitTemplate->getPhysicalDefense() * pow(levelMultipliers, level - 1));
		newUnit->stats["magicDefense"] = (int)((double)unitTemplate->getMagicDefense() * pow(levelMultipliers, level - 1));
		newUnit->stats["speed"] = (int)((double)unitTemplate->getSpeed() * pow(levelMultipliers, level - 1));
		newUnit->stats["numberOfActions"] = (int)((double)unitTemplate->getNumberOfActions() * pow(levelMultipliersForNOA, level - 1));
		newUnit->stats["currentNumberOfActions"] = newUnit->stats["numberOfActions"];
		newUnit->stats["experience"] = (int)((double)unitTemplate->getExperience() * pow(levelMultipliersForEXP, level - 1));
		newUnit->stats["currentExperience"] = 0;
		newUnit->stats["price"] = (int)((double)unitTemplate->getPrice() * pow(levelMultipliers * levelMultipliersForNOA, level - 1));
		newUnit->setLevel(level);
		
		newUnit->statsWithItems = newUnit->stats;
		
		//Incrementing counter
		numberOfCreatedUnits++;
		
		return newUnit;
	}
}

UnitType UnitHandler::translateT(std::string unitTypeString) {
	return translaterSUT[unitTypeString];
}

std::string UnitHandler::translateT(UnitType unitType) {
	return translaterUTS[unitType];
}

void UnitHandler::loadUnitData() {
	std::vector<std::string> unitList = FilesystemHandler::getFilesInDir(basePath);
	
	//NOTE uses file IO
	std::fstream file;
	
	//File pattern:
	//Name
	//TYPE
	//life;
	//meleeDamage;
	//shootingDamage;
	//magicDamage;
	//physicalDefense;
	//magicDefense;
	//speed;
	//numberOfActions;
	//experience;
	//price;
	for (unsigned int i = 0; i < unitList.size(); i++) {
		//Loading the units into level 1 unittemplates
		file.open(basePath + unitList[i], std::ios::in);
		
		AbstractUnit* currentUnit = new AbstractUnit();
		
		std::string name;
		//file >> name;
		std::getline(file, name);
		currentUnit->name = name;
		
		UnitType unitType;
		std::string unitTypeString;
		file >> unitTypeString;
		unitType = translateT(unitTypeString);
		currentUnit->unitType = unitType;
		
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
		file >> currentUnit->price;
		
		baseUnits[name] = currentUnit;
		
		file.close();
		
		if (!file) {
			std::clog << "Error! Unit data might be corrupted (" << name << ")" << std::endl;
		}
	}
	
}

void UnitHandler::translaterInit() {
	//NOTE not compatible with each other
	translaterSUT["FIGHTER"] = UnitType::FIGHTER;
	translaterSUT["ARCHER"] = UnitType::ARCHER;
	translaterSUT["MAGE"] = UnitType::MAGE;
	translaterSUT["OTHER"] = UnitType::OTHER;
	
	translaterUTS[UnitType::FIGHTER] = "Fighter";
	translaterUTS[UnitType::ARCHER] = "Archer";
	translaterUTS[UnitType::MAGE] = "Mage";
	translaterUTS[UnitType::OTHER] = "Other";
}

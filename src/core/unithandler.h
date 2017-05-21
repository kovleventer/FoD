#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

#include "../player/unit.h"
#include "../player/abstractunit.h"
#include "../util/filesystemhandler.h"

/*!
 * @author kovlev
 */


/*!
 * @class UnitHandler loads all unit templates
 * When getUnit is used it generates a new unit based on level
 * Therefore you should handle deletion yourself
 */
class UnitHandler {
public:
	UnitHandler();
	~UnitHandler();
	
	//Creates a new unit
	Unit* getUnit(std::string name, int level);
	
	//Works the same way as the item type and rarity translators
	UnitType translateT(std::string unitTypeString);
	std::string translateT(UnitType unitType);
private:
	void loadUnitData();
	void translaterInit();
	
	std::string basePath;
	//Unit templates for generating real units
	std::map<std::string, AbstractUnit*> baseUnits;
	
	std::map<std::string, UnitType> translaterSUT;
	std::map<UnitType, std::string> translaterUTS;
	
	double levelMultipliers;
	double levelMultipliersForEXP;
	double levelMultipliersForNOA;
	
	//Statistics
	int numberOfCreatedUnits;
};

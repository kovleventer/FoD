#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

#include "unit.h"
#include "abstractunit.h"
#include "filesystemhandler.h"

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
	
	std::map<std::string, UnitType> translaterSIT;
	std::map<UnitType, std::string> translaterITS;
	
	double levelMultipliers;
	double levelMultipliersForEXP;
	double levelMultipliersForNOA;
	
	//Statistics
	int numberOfCreatedUnits;
};

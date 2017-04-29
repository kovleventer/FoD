#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

#include "unit.h"
#include "abstractunit.h"

class UnitHandler {
public:
	UnitHandler();
	~UnitHandler();
	
	//Creates a new unit
	Unit* getUnit(std::string name, int level);
	
private:
	void loadUnitData();
	
	std::string basePath;
	//Unit templates for generating real units
	std::map<std::string, AbstractUnit*> baseUnits;
	
	double levelMultipliers;
	double levelMultipliersForEXP;
	double levelMultipliersForNOA;
	
	//Statistics
	int numberOfCreatedUnits;
};

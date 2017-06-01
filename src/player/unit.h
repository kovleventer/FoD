#pragma once

#include <SDL.h>
#include <string>
#include <map>
#include <numeric>

#include "item.h"
#include "../util/point.h"
#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @enum UnitType 
 * The unit's type (self-explanatory, I think)
 */
enum class UnitType {
	FIGHTER,
	ARCHER,
	MAGE,
	OTHER
};


/*!
 * @class Unit
 * Represents a unit
 * Uses maps with string keys and int values for storing stats
 */
class Unit {
public:
	Unit(std::string n, UnitType uT);
	~Unit();
	
	ATexture* texture;
	
	void render(int x, int y, int w, int h);
	void render(SDL_Rect destinationRect);
	//Renders small stats about the unit
	void renderMiniStats(SDL_Rect destinationRect);
	
	//Getters
	std::string getName();
	int getUnitInventorySize();
	bool getTeamOne();
	Point getPosition();
	bool isDead();
	UnitType getUnitType();
	int getLevel();
	int getTemporaryXP();
	
	//A unit is either melee, or ranged
	bool isMelee();
	bool isRanged();
	
	//Setters
	void setTeamOne(bool newTeamOne);
	void setPositionIndicator(Point newPos);
	void setPositionIndicator(int newX, int newY);
	void setLevel(int newLevel);
	void addTempXP(int xpToAdd);
	void clearTempXP();
	
	//Unit inventory manipulators
	bool addItem(Item* itemToAdd);
	Item* removeItem(int position);
	Item* getItem(int position);
	
	//Default stats for the unit
	std::map<std::string, int> stats;
	//Stats modified by items
	std::map<std::string, int> statsWithItems;
	
	//Wrapper for stats
	int getAttack();
	int getDefense(Unit* attackingUnit);
	
	//Recalculates stats based on items in inventory
	void recalculateInventory();
	
	//Does not return the price of the unit -- it calculates ~ how strong is the unit actually
	int getUnitValue(bool withItems);
	
	//This kills the unit
	void kill();
	
	//Returns true if unit is levelled up
	bool finalizeExperience();
	
	//Static texture for displaying deceased units
	static ATexture* deadTexture;
private:
	std::string name;
	int unitInventorySize;
	
	UnitType unitType;
	
	Item** items;
	
	//Temporary variable to store the parent of the unit
	bool isTeamOne;
	
	Point position;
	
	//Is unit dead
	bool dead;
	
	//The unit's level
	int level;
	
	//Used while in battle
	int temporaryExperienceHolder;
	
	//The font size of the mini stats displayed
	int miniStatsFontSize;
};


/*!
 * @class UnitSpeedComparator
 * Compares two units by their speed stats (used in battle)
 */
class UnitSpeedComparator {
public:
	bool operator()(Unit* a, Unit* b);
};


/*!
 * @class UnitValueComparatorWItems
 * Compares two units by their values with items
 */
class UnitValueComparatorWItems {
public:
	bool operator()(Unit* a, Unit* b);
};


/*!
 * @class UnitValueComparatorNoItems
 * Compares two units by their values without items
 */
class UnitValueComparatorNoItems {
public:
	bool operator()(Unit* a, Unit* b);
};

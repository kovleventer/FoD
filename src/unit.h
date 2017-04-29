#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>

#include "item.h"
#include "point.h"

enum class UnitType {
	FIGHTER,
	ARCHER,
	MAGE,
	OTHER
};

class Unit {
public:
	Unit(std::string n, UnitType uT);
	~Unit();
	
	SDL_Texture* texture;
	
	void render(int x, int y, int w, int h);
	void render(SDL_Rect destinationRect);
	
	//Getters
	std::string getName();
	int getUnitInventorySize();
	bool getTeamOne();
	Point getPosition();
	bool isDead();
	
	//Setters
	void setTeamOne(bool newTeamOne);
	void setPositionIndicator(Point newPos);
	void setPositionIndicator(int newX, int newY);
	
	bool addItem(Item* itemToAdd);
	Item* removeItem(int position);
	Item* getItem(int position);
	
	//Default stats for the unit
	std::map<std::string, int> stats;
	//Stats modified by items
	std::map<std::string, int> statsWithItems;
	
	void recalculateInventory();
	
	//This kills the unit
	void kill();
	
	//Static texture for displaying deceased units
	static SDL_Texture* deadTexture;
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
};

//Compares two units by their speed stats (used in battle)
class UnitSpeedComparator {
public:
	bool operator()(Unit* a, Unit* b);
};

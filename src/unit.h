#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>

#include "item.h"

class Unit {
public:
	Unit(std::string n);
	~Unit();
	
	SDL_Texture* texture;
	
	void render(int x, int y, int w, int h);
	void render(SDL_Rect destinationRect);
	
	//Getters
	std::string getName();
	int getUnitInventorySize();
	
	bool addItem(Item* itemToAdd);
	Item* removeItem(int position);
	Item* getItem(int position);
	
	//Default stats for the unit
	std::map<std::string, int> stats;
	//Stats modified by items
	std::map<std::string, int> statsWithItems;
	
	void recalculateInventory();
private:
	std::string name;
	int unitInventorySize;
	
	Item** items;
};

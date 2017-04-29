#pragma once

#include <SDL2/SDL.h>
#include <sstream>

#include "guipart.h"
#include "unit.h"

class UnitInfo : public GUIPart {
public:
	UnitInfo(int xp, int yp, int wp, int hp);
	
	SDL_Texture* bgTexture;
	
	void render();
	
	//Getters
	Unit* getUnit();
	int getNameSize();
	int getStatsSize();
	
	//Setters
	void setUnit(Unit* newUnit);
	
	//Implementations of virtual methods inherited form superclass
	void handleMousePressEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
private:
	Unit* selectedUnit;
	int nameSize;
	int statsSize;
	
	//Both methods are returning the item
	Item* getItemOnCursor(int xp, int yp);
	Item* removeItemOnCursor(int xp, int yp);
};

#pragma once

#include <SDL2/SDL.h>

#include "guipart.h"
#include "unit.h"
#include "unitinfo.h"
#include "point.h"

enum class UnitAddingPreference {
	FRONTROWFIRST,
	BACKROWFIRST,
	SUPPORTFIRST
};

class Army : public GUIPart {
public:
	//Player init
	Army(int xp, int yp, int wp, int hp, int width, int height, bool isInv);
	
	~Army();
	
	SDL_Texture* bgTexture;
	SDL_Texture* defaultUnitTexture;
	SDL_Texture* selectedUnitTexture;
	SDL_Texture* hoveredUnitTexture;
	
	void render();
	
	//Getters
	int getUnitSize();
	int getPaddingV();
	int getPaddingH();
	UnitInfo* getUnitInfo();
	bool inverted();
	
	//Setters
	void setUnitInfo(UnitInfo* newUnitInfo);
	
	Unit* getUnit(int x, int y);
	Unit* getUnit(Point p);
	
	bool addUnit(Unit* unitToAdd, UnitAddingPreference unitAddingPreference);
	
	void setUnit(int x, int y, Unit* unitToSet);
	void setUnit(Point p, Unit* unitToSet);
	
	//Does not return the removed unit
	void removeUnit(int x, int y);
	void removeUnit(Point p);
	
	void handleMousePressEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	
	void clearHovering();
private:
	Unit** units;
	int iWidth, iHeight;
	unsigned int iSize;
	
	int unitSize;
	int paddingVertical;
	int paddingHorizontal;
	bool isInverted;
	
	void initArray();
	
	Point selectedUnitPos;
	Point hoveredUnitPos;
	
	UnitInfo* unitInfo;
	
	bool addUnitToFrontRow(Unit* unitToAdd);
	bool addUnitToBackRow(Unit* unitToAdd);
	bool addUnitToSupport(Unit* unitToAdd);
	
	//Gets the position in point
	//Returns -1, -1, if no unit is clicked
	Point getUnitOnCursor(int xp, int yp);
};

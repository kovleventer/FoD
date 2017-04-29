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
	
	//Textures
	SDL_Texture* bgTexture;
	SDL_Texture* defaultUnitTexture;
	SDL_Texture* selectedUnitTexture;
	SDL_Texture* hoveredUnitTexture;
	
	void render();
	
	//Getters
	int getWidth();
	int getHeight();
	unsigned int getSize();
	int getUnitSize();
	int getPaddingV();
	int getPaddingH();
	UnitInfo* getUnitInfo();
	bool inverted();
	
	//Setters
	void setUnitInfo(UnitInfo* newUnitInfo);
	void setAllowAttack(bool newAllowAttack);
	void setSelectedUnitPos(Point newSelectedUnitPos);
	
	Unit* getUnit(int x, int y);
	Unit* getUnit(Point p);
	
	bool addUnit(Unit* unitToAdd, UnitAddingPreference unitAddingPreference);
	
	void setUnit(int x, int y, Unit* unitToSet);
	void setUnit(Point p, Unit* unitToSet);
	//On save and load
	void setUnit(int index, Unit* untiToSet);
	
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
	
	//Variable used in battles
	//If true, we can attack an enemy
	bool allowAttack;
};

#pragma once

#include <SDL.h>

#include "../gui/basicgui.h"
#include "unit.h"
#include "unitinfo.h"
#include "../util/point.h"
#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */

/*!
 * @enum UnitAddingPreference to which place should we try to add the unit
 * If the unit can not be added to a position we try another
 */
enum class UnitAddingPreference {
	FRONTROWFIRST,
	BACKROWFIRST,
	SUPPORTFIRST
};


/*!
 * @enum UnitPosition where is the unit
 * This is not the unit's property
 */
enum class UnitPosition {
	FRONTROW,
	BACKROW,
	SUPPORT
};


/*!
 * @class Army represents an army
 * Has a gui, so it is renderable
 * Holds a specified amount of units
 * Interactable by mouse events
 */
class Army : public BasicGUI {
public:
	//Player init
	Army(int xp, int yp, int wp, int hp, int width, int height, bool isInv);
	
	~Army();
	
	//Textures
	ATexture* bgTexture;
	ATexture* defaultUnitTextureFront;
	ATexture* defaultUnitTextureBack;
	ATexture* defaultUnitTextureSupport;
	ATexture* selectedUnitTexture;
	ATexture* hoveredUnitTexture;
	ATexture* attackableUnitTexture;
	ATexture* notAttackableUnitTexture;
	
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
	unsigned char getAttackRestrictionFlags();
	int getEnemyDamageIndicator();
	
	//Setters
	void setUnitInfo(UnitInfo* newUnitInfo);
	void setAllowAttack(bool newAllowAttack);
	void setSelectedUnitPos(Point newSelectedUnitPos);
	void setAttackRestrictionFlags(unsigned char newAttackRestrictionFlags);
	void setEnemyDamageIndicator(int newEnemyDamageIndicator);
	
	Unit* getUnit(int x, int y);
	Unit* getUnit(Point p);
	
	bool addUnit(Unit* unitToAdd);
	bool addUnit(Unit* unitToAdd, UnitAddingPreference unitAddingPreference);
	
	void setUnit(int x, int y, Unit* unitToSet);
	void setUnit(Point p, Unit* unitToSet);
	//On save and load
	void setUnit(int index, Unit* untiToSet);
	
	//Does not return the removed unit
	void removeUnit(int x, int y);
	void removeUnit(Point p);
	
	//Sets all units to NULL
	//NOTE save all pointer values otherwise memory leak will occurs
	void nullifyUnits();
	
	//Switches two units on the given positions
	void switchUnits(Point unitPos1, Point unitPos2);
	
	//Event handling
	void handleLeftClickEvent(int xp, int yp);
	void handleRightClickEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	
	void clearHovering();
	
	//Helper functions for handling battle
	bool isFrontRowEmpty();
	bool isBackRowEmpty();
	bool areFrontAndBackRowsEmpty();
	
	//Gets the first open position on the back/front row
	//Retruns Point(-1, -1) (Point::INVALID), if no such position exists
	Point getFirstOpenFrontRowPosition();
	Point getFirstOpenBackRowPosition();
	
	UnitPosition getUPFromPos(Point pos);
	UnitPosition getUPFromPos(int x, int y);
	
	//FLAGS
	//NOTE the inconvenience of the names
	static const unsigned char ALLOW_FRONTLINE = 0x01; // 0000 0001
	static const unsigned char ALLOW_BACKLINE  = 0x02; // 0000 0010
	static const unsigned char ALLOW_SUPPORT   = 0x04; // 0000 0100
	static const unsigned char ALLOW_ALL       = 0x07; // 0000 0111
	static const unsigned char DISALLOW_ALL    = 0x00; // 0000 0000
	
	unsigned char attackRestrictionFlags;
	
	//Finalizes each units experiences
	void finalizeUnitExperiences();
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
	//Returns -1, -1 (Point::INVALID), if no unit is clicked
	Point getUnitOnCursor(int xp, int yp);
	
	//Variables used in battles
	//If true, we can attack an enemy
	bool allowAttack;
	
	//If true, we can attack the current selected enemy
	bool isEnemyAttackable;
	
	//Renders a rectangle similar to the damage one over the attackable unit
	//This is the parameter of the rectangle
	int enemyDamageIndicator;
};

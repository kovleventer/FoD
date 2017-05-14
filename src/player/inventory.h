#pragma once

#include <SDL.h>

#include "../gui/basicgui.h"
#include "item.h"
#include "../util/point.h"
#include "iteminfo.h"
#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @class Inventory
 * Contains items
 * Has a gui, so it is renderable
 * Handles events, so the items are movable in player's inventory
 */
class Inventory : public BasicGUI {
public:
	//Player init
	Inventory(int xp, int yp, int wp, int hp, int width, int height);
	
	//NPC init
	Inventory(int size);
	
	~Inventory();
	
	ATexture* slotTexture;
	
	void render();
	
	//Getters
	int getSlotSize();
	ItemInfo* getItemInfo();
	int getCurrentHeightPosition();
	unsigned int getInventorySize();
	
	//Setters
	void setItemInfo(ItemInfo* newItemInfo);
	void incrementCurrentHeightPosition();
	void decrementCurrentHeightPosition();
	
	//Returns an item
	Item* getItem(int x, int y);
	Item* getItem(Point p);
	Item* getItem(int index);
	
	//Adds an item if possible
	bool addItem(Item* itemToAdd);
	
	//Sets an item on the given positon to the given new item
	void setItem(int x, int y, Item* itemToSet);
	void setItem(Point p, Item* itemToSet);
	
	//Removes an item and returns it
	Item* removeItem(int x, int y);
	Item* removeItem(Point p);
	
	void handleLeftClickEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	Item** items;
	int iWidth, iHeight;
	//Stores the current height used by scrolling
	int currentHeightPosition;
	unsigned int iSize;
	int slotSize;
	
	//Initilaizes every pointer to null
	void initArray();
	
	//Pointer to the item information gui part on the army tab
	ItemInfo* itemInfo;
};

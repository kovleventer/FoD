#pragma once

#include <SDL2/SDL.h>

#include "guipart.h"
#include "item.h"
#include "point.h"
#include "iteminfo.h"

class Inventory : public GUIPart {
public:
	//Player init
	Inventory(int xp, int yp, int wp, int hp, int width, int height);
	
	//NPC init
	Inventory(int size);
	
	~Inventory();
	
	SDL_Texture* slotTexture;
	
	void render();
	
	//Getters
	int getSlotSize();
	ItemInfo* getItemInfo();
	int getCurrentHeightPosition();
	
	//Setters
	void setItemInfo(ItemInfo* newItemInfo);
	void incrementCurrentHeightPosition();
	void decrementCurrentHeightPosition();
	
	//Returns an item
	Item* getItem(int x, int y);
	Item* getItem(Point p);
	
	//Adds an item if possible
	bool addItem(Item* itemToAdd);
	
	//Sets an item on the given positon to the given new item
	void setItem(int x, int y, Item* itemToSet);
	void setItem(Point p, Item* itemToSet);
	
	//Removes an item and returns it
	Item* removeItem(int x, int y);
	Item* removeItem(Point p);
	
	void handleMousePressEvent(int xp, int yp);
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

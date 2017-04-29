#include "inventory.h"

#include "global.h"

Inventory::Inventory(int xp, int yp, int wp, int hp, int width, int height) : GUIPart(xp, yp, wp, hp) {
	items = new Item*[width * height];
	//Setting size attributes
	iWidth = width;
	iHeight = height;
	iSize = iWidth * iHeight;
	
	slotSize = w / iWidth;
	
	initArray();
	slotTexture = Global::resourceHandler->guiTextures["inventoryslot"];
}

Inventory::Inventory(int size) : GUIPart(0, 0, 0, 0) {
	items = new Item*[size];
	iSize = size;
	
	initArray();
}

Inventory::~Inventory() {
	delete[] items;
}

void Inventory::render() {
	SDL_Rect destinationRect;
	//Fixed params
	destinationRect.w = slotSize;
	destinationRect.h = slotSize;
	
	for (int j = 0; j < iHeight; j++) {
		for (int i = 0; i < iWidth; i++) {
			//Setting rectangle
			destinationRect.x = x + i * slotSize;
			destinationRect.y = y + j * slotSize;
			SDL_RenderCopy(Global::renderer, slotTexture, NULL, &destinationRect);
			if (items[j * iWidth + i] != NULL) {
				items[j * iWidth + i]->render(destinationRect, true);
			}
		}
	}
}

int Inventory::getSlotSize() {
	return slotSize;
}

ItemInfo* Inventory::getItemInfo() {
	return itemInfo;
}

void Inventory::setItemInfo(ItemInfo* newItemInfo) {
	itemInfo = newItemInfo;
}

Item* Inventory::getItem(int x, int y) {
	//Checking for boundaries
	if (y * iWidth + x >= (int)iSize) {
		return NULL;
	} else {
		return items[y * iWidth + x];
	}
}

Item* Inventory::getItem(Point p) {
	return getItem(p.getX(), p.getY());
}

bool Inventory::addItem(Item* itemToAdd) {
	for (unsigned int i = 0; i < iSize; i++) {
		if (items[i] == NULL) {
			items[i] = itemToAdd;
			return true;
		}
	}
	return false;
}

void Inventory::setItem(int x, int y, Item* itemToSet) {
	//Checking for boundaries
	if (y * iWidth + x < (int)iSize) {
		items[y * iWidth + x] = itemToSet;
	}
}

void Inventory::setItem(Point p, Item* itemToSet) {
	setItem(p.getX(), p.getY(), itemToSet);
}

Item* Inventory::removeItem(int x, int y) {
	//Checking for boundaries
	if (y * iWidth + x >= (int)iSize) {
		return NULL;
	} else {
		Item* itemToRemove = items[y * iWidth + x];
		items[y * iWidth + x] = NULL;
		return itemToRemove;
	}
}

Item* Inventory::removeItem(Point p) {
	return removeItem(p.getX(), p.getY());
}

void Inventory::handleMousePressEvent(int xp, int yp) {
	Point rel = Point(xp - x, yp - y);
	rel /= slotSize;
	
	//Switching items
	Item* clickedItem = removeItem(rel);
	setItem(rel, Global::cursor->getItem());
	Global::cursor->setItem(clickedItem);
	
}

void Inventory::handleMouseMotionEvent(int xp, int yp) {
	Point rel = Point(xp - x, yp - y);
	rel /= slotSize;
	itemInfo->setItem(getItem(rel));
}

void Inventory::initArray() {
	//Sets the array with NULLs
	//Memset or something like that would be faster
	for (unsigned int i = 0; i < iSize; i++) {
		items[i] = NULL;
	}
}


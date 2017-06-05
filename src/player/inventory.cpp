#include "inventory.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

Inventory::Inventory(int xp, int yp, int wp, int hp, int width, int height) : BasicGUI(xp, yp, wp, hp) {
	items = new Item*[width * height];
	//Setting size attributes
	iWidth = width;
	iHeight = height;
	currentHeightPosition = 0;
	iSize = iWidth * iHeight;
	
	slotSize = w / iWidth;
	
	initArray();
	slotTexture = Global::resourceHandler->getATexture(TT::GUI, "inventoryslot");
}

Inventory::Inventory(int size) : BasicGUI(0, 0, 0, 0) {
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
	
	//Inventory is always displayed as a square
	for (int j = 0; j < iWidth; j++) {
		for (int i = 0; i < iWidth; i++) {
			//Setting rectangle
			destinationRect.x = x + i * slotSize;
			destinationRect.y = y + j * slotSize;
			slotTexture->render(destinationRect);
			Item* itemToRender = getItem(i, j + currentHeightPosition);
			if (itemToRender != NULL) {
				itemToRender->render(destinationRect, true);
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

int Inventory::getCurrentHeightPosition() {
	return currentHeightPosition;
}

unsigned int Inventory::getInventorySize() {
	return iSize;
}

void Inventory::setItemInfo(ItemInfo* newItemInfo) {
	itemInfo = newItemInfo;
}

void Inventory::incrementCurrentHeightPosition() {
	if (currentHeightPosition < iHeight - iWidth) {
		currentHeightPosition++;
	}
}

void Inventory::decrementCurrentHeightPosition() {
	if (currentHeightPosition > 0) {
		currentHeightPosition--;
	}
}

Item* Inventory::getItem(int x, int y) {
	//Checking for boundaries
	if (y * iWidth + x >= (int)iSize || y * iWidth + x < 0) {
		return NULL;
	} else {
		return items[y * iWidth + x];
	}
}

Item* Inventory::getItem(Point p) {
	return getItem(p.getX(), p.getY());
}

Item* Inventory::getItem(int index) {
	return items[index];
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
	if (y * iWidth + x < (int)iSize || y * iWidth + x < 0) {
		items[y * iWidth + x] = itemToSet;
	}
}

void Inventory::setItem(Point p, Item* itemToSet) {
	setItem(p.getX(), p.getY(), itemToSet);
}

void Inventory::setItem(int index, Item* itemToSell) {
	items[index] = itemToSell;
}

Item* Inventory::removeItem(int x, int y) {
	//Checking for boundaries
	if (y * iWidth + x >= (int)iSize || y * iWidth + x < 0) {
		return NULL;
	} else {
		return removeItem(y * iWidth + x);
	}
}

Item* Inventory::removeItem(Point p) {
	return removeItem(p.getX(), p.getY());
}

Item* Inventory::removeItem(int index) {
	Item* itemToRemove = items[index];
	items[index] = NULL;
	return itemToRemove;
}

void Inventory::handleLeftClickEvent(int xp, int yp) {
	Point rel = Point(xp - x, yp - y);
	rel /= slotSize;
	rel += Point(0, currentHeightPosition);
	
	//Switching items
	Item* clickedItem = removeItem(rel);
	setItem(rel, Global::cursor->getItem());
	Global::cursor->setItem(clickedItem);
	
}

void Inventory::handleMouseMotionEvent(int xp, int yp) {
	Point rel = Point(xp - x, yp - y);
	rel /= slotSize;
	rel += Point(0, currentHeightPosition);
	itemInfo->setItem(getItem(rel));
}

void Inventory::handleMouseWheelEvent(bool up) {
	if (up) {
		Global::player->getInventory()->decrementCurrentHeightPosition();
	} else {
		Global::player->getInventory()->incrementCurrentHeightPosition();
	}
}

void Inventory::initArray() {
	//Sets the array with NULLs
	//Memset or something like that would be faster
	for (unsigned int i = 0; i < iSize; i++) {
		items[i] = NULL;
	}
}


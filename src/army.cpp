#include "army.h"

#include "global.h"

Army::Army(int xp, int yp, int wp, int hp, int width, int height, bool isInv) : GUIPart(xp, yp, wp, hp) {
	units = new Unit*[width * height];
	//Setting size attributes
	iWidth = width;
	iHeight = height;
	iSize = iWidth * iHeight;
	isInverted = isInv;
	
	//NOTE not the best way but whatever
	unitSize = w / iWidth;
	
	initArray();
	
	unitSize = 144;
	paddingVertical = (h - iHeight * unitSize) / (iHeight + 1);
	paddingHorizontal = (w - iWidth * unitSize) / (iWidth + 1);
	
	//Setting textures
	bgTexture = Global::resourceHandler->guiTextures["armybg"];
	defaultUnitTexture = Global::resourceHandler->guiTextures["armyslotbg"];
	selectedUnitTexture = Global::resourceHandler->guiTextures["selectedunit"];
	hoveredUnitTexture = Global::resourceHandler->guiTextures["hoveredunit"];
	
	selectedUnitPos = Point(-1, -1);
	hoveredUnitPos = Point(-1, -1);
}

Army::~Army() {
	for (unsigned int i = 0; i < iSize; i++) {
		if (units[i] != NULL) {
			delete units[i];
		}
	}
	delete[] units;
}

void Army::render() {
	SDL_Rect destinationRect = {x, y, w, h};
	SDL_RenderCopy(Global::renderer, bgTexture, NULL, &destinationRect);
	
	destinationRect.w = unitSize;
	destinationRect.h = unitSize;
	for (int j = 0; j < iHeight; j++) {
		for (int i = 0; i < iWidth; i++) {
			destinationRect.x = x + paddingHorizontal * (i + 1) + unitSize * i;
			destinationRect.y = y + paddingVertical * (j + 1) + unitSize * j;
			if (units[j * iWidth + i] == NULL) {
				SDL_RenderCopy(Global::renderer, defaultUnitTexture, NULL, &destinationRect);
			} else {
				//Rendering of unit selection / hovering indicator
				units[j * iWidth + i]->render(destinationRect);
				if (selectedUnitPos == Point(i, j)) {
					SDL_RenderCopy(Global::renderer, selectedUnitTexture, NULL, &destinationRect);
				} else if (hoveredUnitPos == Point(i, j)) {
					SDL_RenderCopy(Global::renderer, hoveredUnitTexture, NULL, &destinationRect);
				}
			}
		}
	}
}

int Army::getUnitSize() {
	return unitSize;
}

int Army::getPaddingV() {
	return paddingVertical;
}

int Army::getPaddingH() {
	return paddingHorizontal;
}

UnitInfo* Army::getUnitInfo() {
	return unitInfo;
}

bool Army::inverted() {
	return isInverted;
}

void Army::setUnitInfo(UnitInfo* newUnitInfo) {
	unitInfo = newUnitInfo;
}

Unit* Army::getUnit(int x, int y) {
	return units[y * iWidth + x];
}

Unit* Army::getUnit(Point p) {
	return getUnit(p.getX(), p.getY());
}

bool Army::addUnit(Unit* unitToAdd, UnitAddingPreference unitAddingPreference) {
	//We try to add th unit to the given preferred place
	//If that is impossible we are searching for other solutions
	switch(unitAddingPreference) {
		case UnitAddingPreference::FRONTROWFIRST:
			//Front -> Support -> Back
			if (!addUnitToFrontRow(unitToAdd)) {
				if (!addUnitToSupport(unitToAdd)) {
					return addUnitToBackRow(unitToAdd);
				} else {
					return true;
				}
			} else {
				return true;
			}
			break;
		case UnitAddingPreference::BACKROWFIRST:
			//Back -> Support -> Front
			if (!addUnitToBackRow(unitToAdd)) {
				if (!addUnitToSupport(unitToAdd)) {
					return addUnitToFrontRow(unitToAdd);
				} else {
					return true;
				}
			} else {
				return true;
			}
			break;
		case UnitAddingPreference::SUPPORTFIRST:
			//Support -> Back -> Front
			if (!addUnitToSupport(unitToAdd)) {
				if (!addUnitToBackRow(unitToAdd)) {
					return addUnitToFrontRow(unitToAdd);
				} else {
					return true;
				}
			} else {
				return true;
			}
			break;
	}
	return false;
}

void Army::setUnit(int x, int y, Unit* unitToSet) {
	units[y * iWidth + x] = unitToSet;
}

void Army::setUnit(Point p, Unit* unitToSet) {
	setUnit(p.getX(), p.getY(), unitToSet);
}

void Army::removeUnit(int x, int y) {
	//NOTE no checking done
	delete units[y * iWidth + x];
	units[y * iWidth + x] = NULL;
}

void Army::removeUnit(Point p) {
	removeUnit(p.getX(), p.getY());
}

void Army::handleMousePressEvent(int xp, int yp) {
	Point clickPos = getUnitOnCursor(xp, yp);
	if (clickPos != Point(-1, -1)) {
		if (isInverted) {
			//If enemy units are clicked
			
		} else {
			//If player's units are clikced
			//TODO check if in battle and make different behaviours
			
			if (getUnit(clickPos) != NULL && Global::cursor->getItem() != NULL) {
				//If we are currently dragging an item and we click on a unit
				if (getUnit(clickPos)->addItem(Global::cursor->getItem())) {
					getUnit(clickPos)->recalculateInventory();
					Global::cursor->setItem(NULL);
				}
			} else if (clickPos == selectedUnitPos) {
				//When we click again on a selected unit, we remove the selection
				selectedUnitPos = Point(-1, -1);
				unitInfo->setUnit(NULL);
			} else if (selectedUnitPos == Point(-1, -1)) {
				//If we do not have any selected unit, and our current clicked unit is legit
				if (getUnit(clickPos) != NULL) {
					selectedUnitPos = clickPos;
				}
				unitInfo->setUnit(getUnit(clickPos));
			} else {
				//Switching the units
				Unit* tempUnit = getUnit(clickPos);
				setUnit(clickPos, getUnit(selectedUnitPos));
				setUnit(selectedUnitPos, tempUnit);
				selectedUnitPos = Point(-1, -1);
				unitInfo->setUnit(NULL);
			}
		}
	}
}

void Army::handleMouseMotionEvent(int xp, int yp) {
	Point hoverPos = getUnitOnCursor(xp, yp);
	if (hoverPos != Point(-1, -1) && getUnit(hoverPos) != NULL && selectedUnitPos != hoverPos) {
		hoveredUnitPos = hoverPos;
		if (selectedUnitPos == Point(-1, -1)) {
			unitInfo->setUnit(getUnit(hoverPos));
		}
	} else {
		clearHovering();
	}
}

void Army::clearHovering() {
	hoveredUnitPos = Point(-1, -1);
}

void Army::initArray() {
	for (unsigned int i = 0; i < iSize; i++) {
		units[i] = NULL;
	}
}

bool Army::addUnitToFrontRow(Unit* unitToAdd) {
	// .xxx.
	// .....
	for (int i = 1; i < iWidth - 1; i++) {
		if (getUnit(i, 0) == NULL) {
			setUnit(i, 0, unitToAdd);
			return true;
		}
	}
	return false;
}

bool Army::addUnitToBackRow(Unit* unitToAdd) {
	// .....
	// .xxx.
	for (int i = 1; i < iWidth - 1; i++) {
		if (getUnit(i, 1) == NULL) {
			setUnit(i, 1, unitToAdd);
			return true;
		}
	}
	return false;
}

bool Army::addUnitToSupport(Unit* unitToAdd) {
	// x...x
	// x...x
	if (getUnit(0, 0) == NULL) {
		setUnit(0, 0, unitToAdd);
		return true;
	}
	if (getUnit(1, 0) == NULL) {
		setUnit(1, 0, unitToAdd);
		return true;
	}
	if (getUnit(0, iWidth - 1) == NULL) {
		setUnit(0, iWidth - 1, unitToAdd);
		return true;
	}
	if (getUnit(1, iWidth - 1) == NULL) {
		setUnit(1, iWidth - 1, unitToAdd);
		return true;
	}
	return false;
}

Point Army::getUnitOnCursor(int xp, int yp) {
	for (int j = 0; j < iHeight; j++) {
		for (int i = 0; i < iWidth; i++) {
			int unitX = x + paddingHorizontal * (i + 1) + unitSize * i;
			int unitY = y + paddingVertical * (j + 1) + unitSize * j;
			if (xp >= unitX && xp <= unitX + unitSize && yp >= unitY && yp <= unitY + unitSize) {
				return Point(i, j);
			}
		}
	}
	//Returns (-1, -1) if no unit position is clicked
	return Point(-1, -1);
}

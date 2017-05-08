#include "army.h"

#include "global.h"
#include "battle.h"

/*!
 * @author kovlev
 */

Army::Army(int xp, int yp, int wp, int hp, int width, int height, bool isInv) : BasicGUI(xp, yp, wp, hp) {
	units = new Unit*[width * height];
	//Setting size attributes
	iWidth = width;
	iHeight = height;
	iSize = iWidth * iHeight;
	isInverted = isInv;
	
	//NOTE not the best way but whatever
	unitSize = w / iWidth;
	
	initArray();
	
	//Unit size is defined so it hopefully won't cause any errors later
	unitSize = w / (iWidth + 1);
	paddingVertical = (h - iHeight * unitSize) / (iHeight + 1);
	paddingHorizontal = (w - iWidth * unitSize) / (iWidth + 1);
	
	//Setting textures
	if (isInverted) {
		bgTexture = Global::resourceHandler->getATexture(TT::GUI, "armyinvbg");
	} else {
		bgTexture = Global::resourceHandler->getATexture(TT::GUI, "armybg");
	}
	defaultUnitTextureFront = Global::resourceHandler->getATexture(TT::GUI, "armyslotbg_front");
	defaultUnitTextureBack = Global::resourceHandler->getATexture(TT::GUI, "armyslotbg_back");
	defaultUnitTextureSupport = Global::resourceHandler->getATexture(TT::GUI, "armyslotbg_support");
	selectedUnitTexture = Global::resourceHandler->getATexture(TT::GUI, "selectedunit");
	hoveredUnitTexture = Global::resourceHandler->getATexture(TT::GUI, "hoveredunit");
	attackableUnitTexture = Global::resourceHandler->getATexture(TT::GUI, "attackableunit");
	notAttackableUnitTexture = Global::resourceHandler->getATexture(TT::GUI, "notattackableunit");
	
	selectedUnitPos = Point(-1, -1);
	hoveredUnitPos = Point(-1, -1);
	
	//NOTE maybe unneeded, maybe necessary, maybe harmful and can cause errors
	if (isInverted) {
		setUnitInfo(Global::player->getArmy()->getUnitInfo());
	}
	
	allowAttack = false;
	enemyDamageIndicator = 0;
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
	bgTexture->render(destinationRect);
	
	destinationRect.w = unitSize;
	destinationRect.h = unitSize;
	//FIXME correct display and interaction when army is inverted
	/*for (int j0 = 0; j0 < iHeight; j0++) {
		int j = isInverted ? iHeight - 1 - j0 : j0;*/
	for (int j = 0; j < iHeight; j++) {
		for (int i = 0; i < iWidth; i++) {
			destinationRect.x = x + paddingHorizontal * (i + 1) + unitSize * i;
			/*if (isInverted) {
				destinationRect.y = y + paddingVertical * (iHeight - j) + unitSize * (iHeight - 1 - j);
			} else {*/
				destinationRect.y = y + paddingVertical * (j + 1) + unitSize * j;
			//}
			if (units[j * iWidth + i] == NULL || (units[j * iWidth + i]->isDead() && Global::guiHandler->isHardlocked())) {
				//If there are no present units at the given position
				//Or the unit is deceased and we are currently on the battlefield
				//In these cases we render the no-unit-there texture
				
				switch (getUPFromPos(Point(i, j))) {
					case UnitPosition::FRONTROW:
						defaultUnitTextureFront->render(destinationRect);
						break;
					case UnitPosition::BACKROW:
						defaultUnitTextureBack->render(destinationRect);
						break;
					case UnitPosition::SUPPORT:
						defaultUnitTextureSupport->render(destinationRect);
						break;
				}
				
			} else {
				//Rendering of unit selection / hovering indicator
				units[j * iWidth + i]->render(destinationRect);
				if (selectedUnitPos == Point(i, j)) {
					selectedUnitTexture->render(destinationRect);
				} else if (hoveredUnitPos == Point(i, j)) {
					if (isInverted) {
						//If enemy we render an attack indicator texture
						if (isEnemyAttackable && allowAttack) {
							
							//Rendering damage indicator rectangle
							SDL_SetRenderDrawColor(Global::renderer, 0xAF, 0x33, 0x33, 0xAF);
							SDL_Rect damageIndicatorDestinationRect;
							damageIndicatorDestinationRect.x = destinationRect.x;
							damageIndicatorDestinationRect.y = destinationRect.y + (int)((double)(units[j * iWidth + i]->statsWithItems["life"] - units[j * iWidth + i]->statsWithItems["currentLife"]) / units[j * iWidth + i]->statsWithItems["life"] * destinationRect.h);
							damageIndicatorDestinationRect.w = destinationRect.w;
							damageIndicatorDestinationRect.h = (int)((double)(enemyDamageIndicator) / units[j * iWidth + i]->statsWithItems["life"] * destinationRect.h) + 1;
							SDL_RenderFillRect(Global::renderer, &damageIndicatorDestinationRect);
							
							
							//Cleaner this way, maybe we should look for a better solution
							//attackableUnitTexture->render(destinationRect);
						} else {
							notAttackableUnitTexture->render(destinationRect);
						}
					} else {
						hoveredUnitTexture->render(destinationRect);
					}
				}
			}
		}
	}
}

int Army::getWidth() {
	return iWidth;
}

int Army::getHeight() {
	return iHeight;
}

unsigned int Army::getSize() {
	return iSize;
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

unsigned char Army::getAttackRestrictionFlags() {
	return attackRestrictionFlags;
}

int Army::getEnemyDamageIndicator() {
	return enemyDamageIndicator;
}

void Army::setUnitInfo(UnitInfo* newUnitInfo) {
	unitInfo = newUnitInfo;
}

void Army::setAllowAttack(bool newAllowAttack) {
	allowAttack = newAllowAttack;
}

void Army::setSelectedUnitPos(Point newSelectedUnitPos) {
	selectedUnitPos = newSelectedUnitPos;
}

void Army::setAttackRestrictionFlags(unsigned char newAttackRestrictionFlags) {
	attackRestrictionFlags = newAttackRestrictionFlags;
}

void Army::setEnemyDamageIndicator(int newEnemyDamageIndicator) {
	enemyDamageIndicator = newEnemyDamageIndicator;
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
	if (unitToSet != NULL) {
		unitToSet->setPositionIndicator(x, y);
	}
}

void Army::setUnit(Point p, Unit* unitToSet) {
	setUnit(p.getX(), p.getY(), unitToSet);
}

void Army::setUnit(int index, Unit* unitToSet) {
	setUnit(index % iWidth, index / iWidth, unitToSet);
}

void Army::removeUnit(int x, int y) {
	//NOTE no checking done
	delete units[y * iWidth + x];
	units[y * iWidth + x] = NULL;
}

void Army::removeUnit(Point p) {
	removeUnit(p.getX(), p.getY());
}

void Army::switchUnits(Point unitPos1, Point unitPos2) {
	Unit* unitA = getUnit(unitPos1);
	Unit* unitB = getUnit(unitPos2);
	
	setUnit(unitPos1, unitB);
	setUnit(unitPos2, unitA);
}

void Army::handleMousePressEvent(int xp, int yp) {
	Point clickPos = getUnitOnCursor(xp, yp);
	if (clickPos != Point(-1, -1)) {
		if (Global::guiHandler->isHardlocked()) {
			//If we are in a battle
			
			if (allowAttack) {
				//If we can make actions
				
				if (isInverted) {
					//If enemy units are clicked
					
					//isEnemyAttackable is set on mousemotionevent handling
					if (isEnemyAttackable) {
						if (getUnit(clickPos) != NULL && !getUnit(clickPos)->isDead()) {
							allowAttack = false;
							std::thread t([clickPos, this] {
								Global::guiHandler->getBattle()->dealDamage(getUnit(clickPos), enemyDamageIndicator, true);
							});
							t.detach();
						}
					}
				} else {
					//If player's units are clikced
					Unit* tempUnit = getUnit(clickPos);
					if (tempUnit != getUnit(selectedUnitPos)) {
						setUnit(clickPos, getUnit(selectedUnitPos));
						setUnit(selectedUnitPos, tempUnit);
						selectedUnitPos = Point(-1, -1);
						unitInfo->setUnit(NULL);
					}
					allowAttack = false;
					std::thread t([] {
						Global::guiHandler->getBattle()->continueBattle();
					});
					t.detach();
				}
			}
		} else {
			//If player's units are clikced
			
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
	
	if (Global::guiHandler->isHardlocked()) {
		if (hoverPos != Point(-1, -1) && getUnit(hoverPos) != NULL && !getUnit(hoverPos)->isDead()) {
			
			isEnemyAttackable = false;
			switch (getUPFromPos(hoverPos)) {
				case UnitPosition::FRONTROW:
					if (attackRestrictionFlags & ALLOW_FRONTLINE) {
						isEnemyAttackable = true;
					}
					break;
				case UnitPosition::BACKROW:
					if (attackRestrictionFlags & ALLOW_BACKLINE) {
						isEnemyAttackable = true;
					}
					break;
				case UnitPosition::SUPPORT:
					if (attackRestrictionFlags & ALLOW_SUPPORT) {
						isEnemyAttackable = true;
					}
					break;
			}
			
			hoveredUnitPos = hoverPos;
			enemyDamageIndicator = Global::guiHandler->getBattle()->getPossibleDamage(getUnit(hoverPos));
			unitInfo->setUnit(getUnit(hoverPos));
		} else {
			clearHovering();
		}
		return;
	}
	
	if (hoverPos != Point(-1, -1) && getUnit(hoverPos) != NULL && selectedUnitPos != hoverPos) {
		hoveredUnitPos = hoverPos;
		//If we dont have an active unit
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

bool Army::isFrontRowEmpty() {
	for (int i = 1; i < iWidth - 1; i++) {
		if (getUnit(i, 0) != NULL && !getUnit(i, 0)->isDead()) {
			return false;
		}
	}
	return true;
}

bool Army::isBackRowEmpty() {
	for (int i = 1; i < iWidth - 1; i++) {
		if (getUnit(i, 1) != NULL && !getUnit(i, 1)->isDead()) {
			return false;
		}
	}
	return true;
}

bool Army::areFrontAndBackRowsEmpty() {
	return isFrontRowEmpty() && isBackRowEmpty();
}

Point Army::getFirstOpenFrontRowPosition() {
	for (int i = 1; i < iWidth - 1; i++) {
		if (getUnit(i, 0) == NULL || getUnit(i, 0)->isDead()) {
			return Point(i, 0);
		}
	}
	return Point(-1, -1);
}

Point Army::getFirstOpenBackRowPosition() {
	for (int i = 1; i < iWidth - 1; i++) {
		if (getUnit(i, 1) == NULL || getUnit(i, 1)->isDead()) {
			return Point(i, 1);
		}
	}
	return Point(-1, -1);
}

UnitPosition Army::getUPFromPos(Point pos) {
	if (pos == Point(0, 0) || pos == Point(0, 1) ||
		pos == Point(iWidth - 1, 0) || pos == Point(iWidth - 1, 1)) {
		return UnitPosition::SUPPORT;
	}
	if (pos.getY() == 0) {
		return UnitPosition::FRONTROW;
	}
	return UnitPosition::BACKROW;
}

void Army::finalizeUnitExperiences() {
	for (unsigned int i = 0; i < iSize; i++) {
		if (units[i] != NULL) {
			bool isLeveledUp = units[i]->finalizeExperience();
			if (isLeveledUp) {
				int level = units[i]->getLevel();
				
				//Saving inventory
				std::vector<Item*> unitInventory;
				int size = units[i]->getUnitInventorySize();
				for (int j = 0; j < size; j++) {
					unitInventory.push_back(units[i]->getItem(j));
				}
				
				std::string unitName = units[i]->getName();
				
				delete units[i];
				
				units[i] = Global::unitHandler->getUnit(unitName, level + 1);
				for (int j = 0; j < size; j++) {
					units[i]->addItem(unitInventory[j]);
				}
				
				units[i]->recalculateInventory();
			}
		}
	}
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
			unitToAdd->setPositionIndicator(Point(i, 0));
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
			unitToAdd->setPositionIndicator(Point(i, 1));
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
		unitToAdd->setPositionIndicator(Point(0, 0));
		return true;
	}
	if (getUnit(0, 1) == NULL) {
		setUnit(0, 1, unitToAdd);
		unitToAdd->setPositionIndicator(Point(0, 1));
		return true;
	}
	if (getUnit(iWidth - 1, 0) == NULL) {
		setUnit(iWidth - 1, 0, unitToAdd);
		unitToAdd->setPositionIndicator(Point(iWidth - 1, 0));
		return true;
	}
	if (getUnit(iWidth - 1, 1) == NULL) {
		setUnit(iWidth - 1, 1, unitToAdd);
		unitToAdd->setPositionIndicator(Point(iWidth - 1, 1));
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

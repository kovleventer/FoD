#include "unitinfo.h"

#include "global.h"

/*!
 * @author kovlev
 */

UnitInfo::UnitInfo(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	selectedUnit = NULL;
	bgTexture = Global::resourceHandler->getATexture(TT::GUI, "unitinfobg");
	nameSize = 36;
	statsSize = 15;
}

void UnitInfo::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	bgTexture->render(destinationRect);
	
	if (selectedUnit != NULL) {
		destinationRect = {x + w / 10, y + nameSize, w * 4 / 5, w * 4 / 5};
		//Unit
		selectedUnit->render(destinationRect);
		
		int unitTextX = destinationRect.x;
		int unitTextY = destinationRect.y;
		int unitTextWidth = destinationRect.w;
		int unitTextHeight = destinationRect.h;
		
		//Inventory
		//Two rows
		destinationRect.w = destinationRect.w / selectedUnit->getUnitInventorySize() * 2;
		destinationRect.h = destinationRect.h / selectedUnit->getUnitInventorySize() * 2;
		
		for (int i = 0; i < selectedUnit->getUnitInventorySize(); i++) {
			destinationRect.x = unitTextX + unitTextWidth * i / selectedUnit->getUnitInventorySize() - unitTextWidth / selectedUnit->getUnitInventorySize() * ((i) % 2);
			destinationRect.y = unitTextY + unitTextHeight + unitTextHeight * (i % 2) / 2;
			if (selectedUnit->getItem(i) != NULL) {
				selectedUnit->getItem(i)->render(destinationRect, true);
			}
		}
		
		//Name
		ATexture* nameText = Global::resourceHandler->getTextTexture(Text(selectedUnit->getName(), Global::resourceHandler->colors["unitinfo-desc"]));
		Dimension d = nameText->getDimensions();
		d *= nameSize;
		d /= Global::defaultFontSize;
		destinationRect.x = unitTextX + unitTextWidth / 2 - d.W() / 2;
		destinationRect.y = unitTextY - d.H();
		destinationRect.w = d.W();
		destinationRect.h = d.H();
		nameText->render(destinationRect);
		
		//Stats
		for(int i = 0; i <= 9; i++) {
			std::stringstream ossStat;
			std::stringstream ossValue;
			//NOTE should have been done with a translator
			int difference = 0;
			if (i == 0) {
				ossStat << "Life:";
				ossValue << selectedUnit->statsWithItems["currentLife"] << "/" << selectedUnit->statsWithItems["life"];
				difference = selectedUnit->statsWithItems["life"] - selectedUnit->stats["life"];
			} else if (i == 1) {
				ossStat << "Melee Attack:";
				ossValue << selectedUnit->statsWithItems["meleeDamage"];
				difference = selectedUnit->statsWithItems["meleeDamage"] - selectedUnit->stats["meleeDamage"];
			} else if (i == 2) {
				ossStat << "Shooting Attack:";
				ossValue << selectedUnit->statsWithItems["shootingDamage"];
				difference = selectedUnit->statsWithItems["shootingDamage"] - selectedUnit->stats["shootingDamage"];
			} else if (i == 3) {
				ossStat << "Magic Attack:";
				ossValue << selectedUnit->statsWithItems["magicDamage"];
				difference = selectedUnit->statsWithItems["magicDamage"] - selectedUnit->stats["magicDamage"];
			} else if (i == 4) {
				ossStat << "Physical Defense";
				ossValue << selectedUnit->statsWithItems["physicalDefense"];
				difference = selectedUnit->statsWithItems["physicalDefense"] - selectedUnit->stats["physicalDefense"];
			} else if (i == 5) {
				ossStat << "Magic Defense";
				ossValue << selectedUnit->statsWithItems["magicDefense"];
				difference = selectedUnit->statsWithItems["magicDefense"] - selectedUnit->stats["magicDefense"];
			} else if (i == 6) {
				ossStat << "Speed";
				ossValue << selectedUnit->statsWithItems["speed"];
				difference = selectedUnit->statsWithItems["speed"] - selectedUnit->stats["speed"];
			} else if (i == 7) {
				ossStat << "NoA";
				ossValue << selectedUnit->statsWithItems["currentNumberOfActions"] << "/" << selectedUnit->statsWithItems["numberOfActions"];
				difference = selectedUnit->statsWithItems["numberOfActions"] - selectedUnit->stats["numberOfActions"];
			} else if (i == 8) {
				ossStat << "Experience";
				ossValue << selectedUnit->statsWithItems["currentExperience"] << "/" << selectedUnit->statsWithItems["experience"];
			} else if (i == 9) {
				ossStat << "Level";
				ossValue << selectedUnit->getLevel();
			}
			
			ATexture* statText = Global::resourceHandler->getTextTexture(Text(ossStat.str(), Global::resourceHandler->colors["unitinfo-desc"]));
			d = statText->getDimensions();
			d *= statsSize;
			d /= Global::defaultFontSize;
			destinationRect.x = unitTextX;
			destinationRect.y = unitTextY + unitTextHeight * 2 + i * (d.H() + 10);
			destinationRect.w = d.W();
			destinationRect.h = d.H();
			statText->render(destinationRect);
			
			
			SDL_Color valueColor;
			if (difference == 0) {
				valueColor = Global::resourceHandler->colors["unitinfo-values-unchanged"];
			} else if (difference > 0) {
				valueColor = Global::resourceHandler->colors["unitinfo-values-incremented"];
			} else {
				valueColor = Global::resourceHandler->colors["unitinfo-values-decremented"];
			}
			
			ATexture* valueText = Global::resourceHandler->getTextTexture(Text(ossValue.str(), valueColor));
			d = valueText->getDimensions();
			d *= statsSize;
			d /= Global::defaultFontSize;
			destinationRect.x = unitTextX + unitTextWidth - d.W();
			destinationRect.y = unitTextY + unitTextHeight * 2 + i * (d.H() + 10);
			destinationRect.w = d.W() * 1.1;
			destinationRect.h = d.H() * 1.1;
			valueText->render(destinationRect);
		}
	}
}

Unit* UnitInfo::getUnit() {
	return selectedUnit;
}

int UnitInfo::getNameSize() {
	return nameSize;
}

int UnitInfo::getStatsSize() {
	return statsSize;
}

void UnitInfo::setUnit(Unit* newUnit) {
	selectedUnit = newUnit;
}

void UnitInfo::handleMousePressEvent(int xp, int yp) {
	//On battle it does nothing
	if (Global::guiHandler->isHardlocked()) return;
	
	if (Global::cursor->getItem() == NULL) {
		Global::cursor->setItem(removeItemOnCursor(xp, yp));
		if (selectedUnit != NULL) {
			selectedUnit->recalculateInventory();
		}
	}
}

void UnitInfo::handleMouseMotionEvent(int xp, int yp) {
	Global::player->getInventory()->getItemInfo()->setItem(getItemOnCursor(xp, yp));
}

//FIXME possible code duplication
Item* UnitInfo::getItemOnCursor(int xp, int yp) {
	if (selectedUnit == NULL) return NULL;
	
	if (x + w / 10 < xp && x + w / 10 + w * 4 / 5 >= xp && y + nameSize + w * 4 / 5 < yp && y + nameSize + w * 8 / 5 >= yp) {
		//Middle point of the unit's inventory
		int xMid = x + w / 2;
		int yMid = ((y + nameSize + w * 4 / 5) + (y + nameSize + w * 8 / 5)) / 2;
		if (xMid < xp) {
			if (yMid < yp) {
				return selectedUnit->getItem(3);
			} else {
				return selectedUnit->getItem(2);
			}
		} else {
			if (yMid < yp) {
				return selectedUnit->getItem(1);
			} else {
				return selectedUnit->getItem(0);
			}
		}
	} else {
		return NULL;
	}
}

Item* UnitInfo::removeItemOnCursor(int xp, int yp) {
	if (selectedUnit == NULL) return NULL;
	
	if (x + w / 10 < xp && x + w / 10 + w * 4 / 5 >= xp && y + nameSize + w * 4 / 5 < yp && y + nameSize + w * 8 / 5 >= yp) {
		//Middle point of the unit's inventory
		int xMid = x + w / 2;
		int yMid = ((y + nameSize + w * 4 / 5) + (y + nameSize + w * 8 / 5)) / 2;
		if (xMid < xp) {
			if (yMid < yp) {
				return selectedUnit->removeItem(3);
			} else {
				return selectedUnit->removeItem(2);
			}
		} else {
			if (yMid < yp) {
				return selectedUnit->removeItem(1);
			} else {
				return selectedUnit->removeItem(0);
			}
		}
	} else {
		return NULL;
	}
}

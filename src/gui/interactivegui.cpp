#include "interactivegui.h"

#include "../core/global.h"
#include "../map/worldobject.h"

/*!
 * @author kovlev
 */

InteractiveGUI::InteractiveGUI(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	partChooserWidth = w / 5;
	padding = 15;
	currentPartIndex = 0;
	
	chooserText = Global::resourceHandler->getATexture(TT::GUI, "interactivechooser");
}

InteractiveGUI::InteractiveGUI(SDL_Rect dimensionRect) : InteractiveGUI(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

InteractiveGUI::~InteractiveGUI() {
	for (unsigned int i = 0; i < parts.size(); i++) {
		delete parts[i].first;
		delete parts[i].second;
	}
}

void InteractiveGUI::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, partChooserWidth, h};
	chooserText->render(destinationRect);
	
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i].second->getIsRenderableWhenNotOwnedByPlayer() || dynamic_cast<Player*>(parent->getOwner()) != NULL) {
			parts[i].first->render();
			if (i == currentPartIndex) {
				parts[i].second->render();
			}
		}
	}
}

int InteractiveGUI::getPartChooserWidth() {
	return partChooserWidth;
}

int InteractiveGUI::getPadding() {
	return padding;
}

unsigned int InteractiveGUI::getCurrentPartIndex() {
	return currentPartIndex;
}

unsigned int InteractiveGUI::getPartCount() {
	return parts.size();
}

WholeScreenGUI* InteractiveGUI::getPart(unsigned int index) {
	//NULL checking
	if (parts.size() > index) {
		return parts[index].second;
	}
	return NULL;
}

WholeScreenGUI* InteractiveGUI::getCurrentPart() {
	return getPart(currentPartIndex);
}

SDL_Rect InteractiveGUI::getRemainingDim() {
	return {x + partChooserWidth, y, w - partChooserWidth, h};
}

InteractiveWorldObject* InteractiveGUI::getParent() {
	return parent;
}

void InteractiveGUI::setParent(InteractiveWorldObject* newParent) {
	parent = newParent;
}

void InteractiveGUI::addPart(std::pair<std::string, WholeScreenGUI*> newPart) {
	int bHeight = h / 10;
	Button* currentButton = new Button(x + padding, y + padding + parts.size() * (padding + bHeight), partChooserWidth - 2 * padding, bHeight);
	currentButton->setText(newPart.first);
	newPart.second->setHeaderText(newPart.first);
	parts.push_back({currentButton, newPart.second});
}

void InteractiveGUI::handleLeftClickEvent(int xp, int yp) {
	if (partChooserWidth + x > xp) {
		for (unsigned int i = 0; i < parts.size(); i++) {
			if (parts[i].first->contains(xp, yp)) {
				currentPartIndex = i;
				break;
			}
		}
	} else {
		parts[currentPartIndex].second->handleLeftClickEvent(xp, yp);
	}
}

void InteractiveGUI::handleRightClickEvent(int xp, int yp) {
	if (partChooserWidth + x > xp) {
		for (unsigned int i = 0; i < parts.size(); i++) {
			if (parts[i].first->contains(xp, yp)) {
				currentPartIndex = i;
				break;
			}
		}
	} else {
		parts[currentPartIndex].second->handleRightClickEvent(xp, yp);
	}
}

void InteractiveGUI::handleMouseMotionEvent(int xp, int yp) {
	parts[currentPartIndex].second->handleMouseMotionEvent(xp, yp);
}

void InteractiveGUI::handleMouseWheelEvent(bool up) {
	parts[currentPartIndex].second->handleMouseWheelEvent(up);
}

// Specialized parts begins here

ItemBuyingMenu::ItemBuyingMenu(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	numberOfDisplayableItems = 5;
	itemSlotHeight = h / numberOfDisplayableItems;
	padding = 1;
	currentItemPosition = 0;
	selectedItemPosition = 0;
	itemBgText = Global::resourceHandler->getATexture(TT::GUI, "marketitembg");
	selectedItemBgText = Global::resourceHandler->getATexture(TT::GUI, "marketitemselectedbg");
	fontSize = 28;
	itemInfo = NULL;
	itemCheckoutMenu = NULL;
}

ItemBuyingMenu::ItemBuyingMenu(SDL_Rect dimensionRect) : ItemBuyingMenu(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

void ItemBuyingMenu::render() {
	//Setting rectangle
	SDL_Rect destinationRect;
	destinationRect.x = x;
	destinationRect.w = w;
	destinationRect.h = itemSlotHeight;
	for (int i = 0; i < numberOfDisplayableItems; i++) {
		destinationRect.y = y + i * itemSlotHeight;
		//Rendering background
		if (selectedItemPosition == i + currentItemPosition) {
			selectedItemBgText->render(destinationRect);
		} else {
			itemBgText->render(destinationRect);
		}
		
		//Rendering current item if possible
		Item* currentRenderedItem = getItem(i + currentItemPosition);
		if (currentRenderedItem != NULL) {
			//Rendering the item itself
			SDL_Rect itemDestionationRect = destinationRect;
			itemDestionationRect.x += padding;
			itemDestionationRect.y += padding;
			itemDestionationRect.h = itemSlotHeight - 2 * padding;
			itemDestionationRect.w = itemDestionationRect.h;
			currentRenderedItem->render(itemDestionationRect, true);
			
			//Rendering item name
			ATexture* itemNameText = Global::resourceHandler->getTextTexture(currentRenderedItem->getName(), Global::resourceHandler->getColor("whole-header"));
			Dimension d = itemNameText->getDimensions();
			d *= fontSize;
			d /= Global::defaultFontSize;
			itemDestionationRect.x += padding + itemDestionationRect.w;
			itemDestionationRect.w = d.W();
			itemDestionationRect.h = d.H();
			itemNameText->render(itemDestionationRect);
			
			//Rendering item price
			ATexture* itemPriceText = Global::resourceHandler->getTextTexture(std::to_string(currentRenderedItem->getPrice()), Global::resourceHandler->getColor("whole-header"));
			d = itemPriceText->getDimensions();
			d *= fontSize;
			d /= Global::defaultFontSize;
			itemDestionationRect.x = destinationRect.x + destinationRect.w - padding - d.W();
			itemDestionationRect.w = d.W();
			itemDestionationRect.h = d.H();
			itemPriceText->render(itemDestionationRect);
		}
	}
}

Item* ItemBuyingMenu::getItem(unsigned int index) {
	//NULL checking
	if (index < itemsToSell.size()) {
		return itemsToSell[index];
	}
	return NULL;
}

unsigned int ItemBuyingMenu::getItemsToSellSize() {
	return itemsToSell.size();
}

void ItemBuyingMenu::addItem(Item* itemToAdd) {
	itemsToSell.push_back(itemToAdd);
}

void ItemBuyingMenu::removeCurrentItem() {
	itemsToSell.erase(itemsToSell.begin() + selectedItemPosition);
	selectedItemPosition = 0;
}

void ItemBuyingMenu::setItemList(std::vector<Item*> newList) {
	itemsToSell = newList;
}

int ItemBuyingMenu::getFontSize() {
	return fontSize;
}

int ItemBuyingMenu::getPadding() {
	return padding;
}

ItemInfo* ItemBuyingMenu::getItemInfo() {
	return itemInfo;
}

ItemCheckoutMenu* ItemBuyingMenu::getItemCheckoutMenu() {
	return itemCheckoutMenu;
}

void ItemBuyingMenu::setFontSize(int newFontSize) {
	fontSize = newFontSize;
}

void ItemBuyingMenu::setItemInfo(ItemInfo* newItemInfo) {
	itemInfo = newItemInfo;
}

void ItemBuyingMenu::setItemCheckoutMenu(ItemCheckoutMenu* newItemCheckoutMenu) {
	itemCheckoutMenu = newItemCheckoutMenu;
}

void ItemBuyingMenu::setSelectedItemPosition(unsigned int newSelectedItemPosition) {
	selectedItemPosition = newSelectedItemPosition;
}

void ItemBuyingMenu::handleLeftClickEvent(int xp, int yp) {
	selectedItemPosition = (yp - y) / itemSlotHeight + currentItemPosition;
	itemInfo->setItem(getItem(selectedItemPosition));
	itemCheckoutMenu->setCurrentItemToBuy(getItem(selectedItemPosition));
}

void ItemBuyingMenu::handleMouseWheelEvent(bool up) {
	if (up) {
		if (currentItemPosition > 0) {
			currentItemPosition--;
		}
	} else {
		if (currentItemPosition + numberOfDisplayableItems < itemsToSell.size()) { // Avoiding negative values
			currentItemPosition++;
		}
	}
}

ItemCheckoutMenu::ItemCheckoutMenu(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	goldText = Global::resourceHandler->getATexture(TT::GUI, "gold");
	bgText = Global::resourceHandler->getATexture(TT::GUI, "marketcheckoutbg");
	horizontalPadding = 10;
	verticalPadding = 10;
	rowHeight = (h - 6 * verticalPadding) / 3;
	fontSize = 30;
	currentItemToBuy = NULL;
	currentItemToSell = NULL;
	buyButton = new Button(x + w - horizontalPadding - rowHeight,
							y + verticalPadding, rowHeight, rowHeight);
	buyButton->setText("Buy");
	sellButton = new Button(x + w - horizontalPadding - rowHeight,
							y + verticalPadding * 5 + rowHeight * 2, rowHeight, rowHeight);
	sellButton->setText("Sell");
	itemBuyingMenu = NULL;
	taxRatio = 0.9;
}

ItemCheckoutMenu::ItemCheckoutMenu(SDL_Rect dimensionRect) : ItemCheckoutMenu(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

ItemCheckoutMenu::~ItemCheckoutMenu() {
	//Deletes buttons
	delete buyButton;
	delete sellButton;
}

void ItemCheckoutMenu::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	//Rendering background
	bgText->render(destinationRect);
	
	int itemToBuyPrice, itemToSellPrice;
	
	//Rendering item to buy
	destinationRect.x += horizontalPadding;
	destinationRect.y += verticalPadding;
	destinationRect.w = rowHeight;
	destinationRect.h = rowHeight;
	if (currentItemToBuy != NULL) {
		currentItemToBuy->render(destinationRect, true);
		itemToBuyPrice = currentItemToBuy->getPrice();
	}
	
	//Rendering gold icon
	destinationRect.y += 2 * verticalPadding + rowHeight;
	goldText->render(destinationRect);
	
	//Rendering item to sell
	destinationRect.y += 2 * verticalPadding + rowHeight;
	if (currentItemToSell != NULL) {
		currentItemToSell->render(destinationRect, true);
		itemToSellPrice = currentItemToSell->getPrice() * taxRatio;
	}
	
	Dimension d;
	//Rendering item price textures
	ATexture* itemPriceTextB;
	if (Global::player->getGold() >= itemToBuyPrice) {
		itemPriceTextB = Global::resourceHandler->getTextTexture(std::to_string(itemToBuyPrice), Global::resourceHandler->getColor("unitinfo-values-unchanged"));
	} else {
		itemPriceTextB = Global::resourceHandler->getTextTexture(std::to_string(itemToBuyPrice), Global::resourceHandler->getColor("unitinfo-values-decremented"));
	}
	d = itemPriceTextB->getDimensions();
	d *= fontSize;
	d /= Global::defaultFontSize;
	destinationRect.x = x + w / 2 - d.W() / 2;
	destinationRect.y = y + verticalPadding + rowHeight / 2 - d.H() / 2;
	destinationRect.w = d.W();
	destinationRect.h = d.H();
	if (currentItemToBuy != NULL) {
		itemPriceTextB->render(destinationRect);
	}
	
	ATexture* goldValueText = Global::resourceHandler->getTextTexture(std::to_string(Global::player->getGold()), Global::resourceHandler->getColor("gold"));
	d = goldValueText->getDimensions();
	d *= fontSize;
	d /= Global::defaultFontSize;
	destinationRect.x = x + w / 2 - d.W() / 2;
	destinationRect.y += 2 * verticalPadding + rowHeight;
	destinationRect.w = d.W();
	destinationRect.h = d.H();
	goldValueText->render(destinationRect);
	
	ATexture* itemPriceTextS = Global::resourceHandler->getTextTexture(std::to_string(itemToSellPrice), Global::resourceHandler->getColor("unitinfo-values-incremented"));
	d = itemPriceTextS->getDimensions();
	d *= fontSize;
	d /= Global::defaultFontSize;
	destinationRect.x = x + w / 2 - d.W() / 2;
	destinationRect.y += 2 * verticalPadding + rowHeight;
	destinationRect.w = d.W();
	destinationRect.h = d.H();
	if (currentItemToSell != NULL) {
		itemPriceTextS->render(destinationRect);
	}
	
	buyButton->render();
	sellButton->render();
}

Item* ItemCheckoutMenu::getCurrentItemToBuy() {
	return currentItemToBuy;
}

Item* ItemCheckoutMenu::getCurrentItemToSell() {
	return currentItemToSell;
}

int ItemCheckoutMenu::getPaddingH() {
	return horizontalPadding;
}

int ItemCheckoutMenu::getPaddingV() {
	return verticalPadding;
}

int ItemCheckoutMenu::getRowHeight() {
	return rowHeight;
}

ItemBuyingMenu* ItemCheckoutMenu::getItemBuyingMenu() {
	return itemBuyingMenu;
}

void ItemCheckoutMenu::setCurrentItemToBuy(Item* newItemToBuy) {
	currentItemToBuy = newItemToBuy;
}

void ItemCheckoutMenu::setCurrentItemToSell(Item* newItemToSell) {
	currentItemToSell = newItemToSell;
}

void ItemCheckoutMenu::setItemBuyingMenu(ItemBuyingMenu* newItemBuyingMenu) {
	itemBuyingMenu = newItemBuyingMenu;
}

void ItemCheckoutMenu::handleLeftClickEvent(int xp, int yp) {
	//Checing button clicking
	//NOTE very ugly
	if (buyButton->contains(xp, yp)) {
		if (currentItemToBuy != NULL) {
			int itemToBuyPrice = currentItemToBuy->getPrice();
			if (Global::player->getGold() >= itemToBuyPrice) {
				Global::player->takeGold(itemToBuyPrice);
				Global::player->getInventory()->addItem(currentItemToBuy);
				currentItemToBuy = NULL;
				itemBuyingMenu->removeCurrentItem();
			}
		}
		return;
	}
	
	if (sellButton->contains(xp, yp)) {
		if (currentItemToSell != NULL) {
			int itemToSellPrice = currentItemToSell->getPrice() * taxRatio;
			Global::player->giveGold(itemToSellPrice);
			currentItemToSell = NULL;
		}
		return;
	}
	
	Item* currItemOnCursor = Global::cursor->getItem();
	Global::cursor->setItem(currentItemToSell);
	currentItemToSell = currItemOnCursor;
}

UnitBuyingMenu::UnitBuyingMenu(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	bgText = Global::resourceHandler->getATexture(TT::GUI, "unitbuyingbg");
	currentUnitPosition = 0;
	numberOfDisplayableUnits = Global::player->getArmy()->getWidth();
	
	//Initializing buttons
	buyButtons = new Button*[numberOfDisplayableUnits];
	for (unsigned int i = 0; i < numberOfDisplayableUnits; i++) {
		buyButtons[i] = new Button(0, 0, 0, 0);
		buyButtons[i]->setText("Buy");
	}
	
	visibleUnitCount = 0;
	unitSize = Global::player->getArmy()->getUnitSize();
	paddingH = 0; // Might be changed
	paddingV = (h - unitSize) / 2; // Will not be changed
	
	fontSize = 28;
}

UnitBuyingMenu::UnitBuyingMenu(SDL_Rect dimensionRect) : UnitBuyingMenu(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

UnitBuyingMenu::~UnitBuyingMenu() {
	//Deleting remaining units
	for (unsigned int i = 0; i < unitsToSell.size(); i++) {
		delete unitsToSell[i];
	}
	
	//Deleting buttons
	for (unsigned int i = 0; i < numberOfDisplayableUnits; i++) {
		delete buyButtons[i];
	}
	delete[] buyButtons;
}

void UnitBuyingMenu::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	bgText->render(destinationRect);
	
	destinationRect.y += paddingV;
	for (int i = 0; i < visibleUnitCount; i++) {
		destinationRect.x += paddingH;
		destinationRect.w = unitSize;
		destinationRect.h = unitSize;
		
		unitsToSell[i + currentUnitPosition]->render(destinationRect);
		
		buyButtons[i]->render();
		
		ATexture* priceText = Global::resourceHandler->getTextTexture(std::to_string(unitsToSell[i + currentUnitPosition]->statsWithItems["price"]),
																	  Global::resourceHandler->getColor("whole-header"));
		Dimension d = priceText->getDimensions();
		d *= fontSize;
		d /= Global::defaultFontSize;
		SDL_Rect textDestinationRect = destinationRect;
		textDestinationRect.w = d.W();
		textDestinationRect.h = d.H();
		textDestinationRect.y -= d.H() + 10;
		textDestinationRect.x += unitSize / 2 - d.W() / 2;
		priceText->render(textDestinationRect);
		
		destinationRect.x += unitSize;
	}
}

Unit* UnitBuyingMenu::getUnit(unsigned int index) {
	//NULL checking
	if (index < unitsToSell.size()) {
		return unitsToSell[index];
	}
	return NULL;
}

unsigned int UnitBuyingMenu::getUnitsToSellSize() {
	return unitsToSell.size();
}

void UnitBuyingMenu::addUnit(Unit* unitToAdd) {
	unitsToSell.push_back(unitToAdd);
	if (unitsToSell.size() <= 5) {
		recalcPositions();
	}
}

void UnitBuyingMenu::removeUnit(unsigned int index) {
	//Does not actually delete the unit
	unitsToSell.erase(unitsToSell.begin() + index);
	
	//Checking indexes
	if (unitsToSell.size() < 5) {
		recalcPositions();
	}
	if (unitsToSell.size() <= 5) {
		currentUnitPosition = 0;
	} else {
		if (currentUnitPosition + numberOfDisplayableUnits >= unitsToSell.size()) {
			currentUnitPosition--;
		}
	}
}

void UnitBuyingMenu::setUnitList(std::vector<Unit*> newList) {
	if (unitsToSell.size() == 0) {
		unitsToSell = newList;
		recalcPositions();
	} else {
		//When we are overwriting units
		std::clog << "Warning: Trying to overwrite non empty unit list. Overwritten units are deleted." << std::endl;
		for (unsigned int i = 0; i < unitsToSell.size(); i++) {
			delete unitsToSell[i];
		}
		unitsToSell = newList;
		recalcPositions();
	}
}

void UnitBuyingMenu::handleLeftClickEvent(int xp, int yp) {
	for (int i = 0; i < visibleUnitCount; i++) {
		//If we clicked on the buy button
		if (buyButtons[i]->contains(xp, yp)) {
			
			//Price checking
			int price = getUnit(currentUnitPosition + i)->statsWithItems["price"];
			if (price <= Global::player->getGold()) {
				Unit* unitToAdd = getUnit(currentUnitPosition + i);
				
				//We do not purchase the unit if our army is full
				if (Global::player->getArmy()->addUnit(unitToAdd)) {
					removeUnit(currentUnitPosition + i);
					Global::player->takeGold(price);
				}
			}
			break;
		}
	}
}

void UnitBuyingMenu::handleMouseWheelEvent(bool up) {
	//Scrolls the menu to show more units
	if (unitsToSell.size() > 5) {
		if (up) {
			if (currentUnitPosition > 0) {
				currentUnitPosition--;
			}
		} else {
			if (currentUnitPosition + numberOfDisplayableUnits < unitsToSell.size()) { // Avoiding negative values
				currentUnitPosition++;
			}
		}
	}
}

void UnitBuyingMenu::recalcPositions() {
	visibleUnitCount = unitsToSell.size() <= 5 ? unitsToSell.size() : 5;
	paddingH = (w - visibleUnitCount * unitSize) / (visibleUnitCount + 1);
	
	SDL_Rect helperRect = {x, y + paddingV + unitSize + 10, unitSize, unitSize / 3};
	for (int i = 0; i < visibleUnitCount; i++) {
		helperRect.x += paddingH;
		
		buyButtons[i]->setDimensionRect(helperRect);
		
		helperRect.x += unitSize;
	}
}

Garrison::Garrison(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	garrisonArmy = NULL;
	recreateGarrisonArmy();
}

Garrison::Garrison(SDL_Rect dimensionRect) : Garrison(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

Garrison::~Garrison() {
	delete garrisonArmy;
}

void Garrison::render() {
	garrisonArmy->render();
}

Army* Garrison::getArmy() {
	return garrisonArmy;
}

void Garrison::recreateGarrisonArmy() {
	if (garrisonArmy != NULL) {
		delete garrisonArmy;
	}
	garrisonArmy = new Army(x, y, w, h, Global::player->getArmy()->getWidth(), Global::player->getArmy()->getHeight(), true);
}

void Garrison::handleLeftClickEvent(int xp, int yp) {
	//Simple passthrough
	garrisonArmy->handleLeftClickEvent(xp, yp);
}

void Garrison::handleMouseMotionEvent(int xp, int yp) {
	//Simple passthrough
	garrisonArmy->handleMouseMotionEvent(xp, yp);
}

ItemMarket::ItemMarket(InteractiveGUI* parent) : WholeScreenGUI(parent->getRemainingDim()) {
	inventory = Global::player->getInventory();
	itemBuyingMenu = new ItemBuyingMenu(inventory->getItemInfo()->getDimensionRect());
	itemInfo = new ItemInfo(itemBuyingMenu->getX(), itemBuyingMenu->getY() + itemBuyingMenu->getH(), itemBuyingMenu->getW(), itemBuyingMenu->getH());
	itemBuyingMenu->setItemInfo(itemInfo);
	itemCheckoutMenu = new ItemCheckoutMenu(itemInfo->getX() - itemInfo->getW(), itemInfo->getY(), itemInfo->getW(), itemInfo->getH());
	itemBuyingMenu->setItemCheckoutMenu(itemCheckoutMenu);
	itemCheckoutMenu->setItemBuyingMenu(itemBuyingMenu);
	
	addTempPart(itemBuyingMenu);
	addTempPart(itemCheckoutMenu);
	addTempPart(itemInfo);
	addPart(inventory);
}

ItemBuyingMenu* ItemMarket::getItemBuyingMenu() {return itemBuyingMenu;}
ItemCheckoutMenu* ItemMarket::getItemCheckoutMenu() {return itemCheckoutMenu;}
ItemInfo* ItemMarket::getItemInfo() {return itemInfo;}
Inventory* ItemMarket::getInventory() {return inventory;}

Barracks::Barracks(InteractiveGUI* parent) : WholeScreenGUI(parent->getRemainingDim()) {
	army = Global::player->getArmy();
	unitBuyingMenu = new UnitBuyingMenu(Global::player->getInventory()->getX(), Global::player->getInventory()->getY(),
												 Global::player->getInventory()->getW() * 2, Global::player->getInventory()->getH());
	
	addTempPart(unitBuyingMenu);
	addPart(army);
}

UnitBuyingMenu* Barracks::getUnitBuyingMenu() {return unitBuyingMenu;}
Army* Barracks::getArmy() {return army;}

GarrisonWrapper::GarrisonWrapper(InteractiveGUI* parent) : WholeScreenGUI(parent->getRemainingDim()) {
	army = Global::player->getArmy();
	garrison = new Garrison(army->getX(), parent->getY() + headerSize,
							army->getW(), army->getY() - parent->getY() - headerSize);
	
	addTempPart(garrison);
	addPart(army);
}

Garrison* GarrisonWrapper::getGarrison() {return garrison;}
Army* GarrisonWrapper::getArmy() {return army;}

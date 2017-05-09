#include "interactivegui.h"

#include "../global.h"

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
		parts[i].first->render();
		if (i == currentPartIndex) {
			parts[i].second->render();
		}
	}
}

int InteractiveGUI::getPartChooserWidth() {
	return partChooserWidth;
}

int InteractiveGUI::getPadding() {
	return padding;
}

SDL_Rect InteractiveGUI::getRemainingDim() {
	return {x + partChooserWidth, y, w - partChooserWidth, h};
}

void InteractiveGUI::addPart(std::pair<std::string, WholeScreenGUI*> newPart) {
	int bHeight = h / 10;
	Button* currentButton = new Button(x + padding, y + padding + parts.size() * (padding + bHeight), partChooserWidth - 2 * padding, bHeight);
	currentButton->setText(newPart.first);
	newPart.second->setHeaderText(newPart.first);
	parts.push_back({currentButton, newPart.second});
}

void InteractiveGUI::handleMousePressEvent(int xp, int yp) {
	if (partChooserWidth + x > xp) {
		for (unsigned int i = 0; i < parts.size(); i++) {
			if (parts[i].first->contains(xp, yp)) {
				currentPartIndex = i;
				break;
			}
		}
	} else {
		parts[currentPartIndex].second->handleMousePressEvent(xp, yp);
	}
}

void InteractiveGUI::handleMouseMotionEvent(int xp, int yp) {
	parts[currentPartIndex].second->handleMouseMotionEvent(xp, yp);
}

void InteractiveGUI::handleMouseWheelEvent(bool up) {
	parts[currentPartIndex].second->handleMouseWheelEvent(up);
}

// Specialized parts begins here

TestGUIPart::TestGUIPart(InteractiveGUI* parent) : WholeScreenGUI(parent->getRemainingDim()) {
	addPart(Global::player->getInventory());
	ItemBuyingMenu* tempIBM = new ItemBuyingMenu(Global::player->getInventory()->getItemInfo()->getDimensionRect());
	tempIBM->addItem(Global::itemHandler->getItem("sword"));
	tempIBM->addItem(Global::itemHandler->getItem("sickle"));
	tempIBM->addItem(Global::itemHandler->getItem("shield"));
	tempIBM->addItem(Global::itemHandler->getItem("mshield1"));
	tempIBM->addItem(Global::itemHandler->getItem("mshield2"));
	tempIBM->addItem(Global::itemHandler->getItem("bow"));
	ItemInfo* tempII = new ItemInfo(tempIBM->getX(), tempIBM->getY() + tempIBM->getH(), tempIBM->getW(), tempIBM->getH());
	tempIBM->setItemInfo(tempII);
	ItemCheckoutMenu* tempICM = new ItemCheckoutMenu(tempII->getX() - tempII->getW(), tempII->getY(), tempII->getW(), tempII->getH());
	tempIBM->setItemCheckoutMenu(tempICM);
	tempICM->setItemBuyingMenu(tempIBM);
	
	addTempPart(tempIBM);
	addTempPart(tempII);
	addTempPart(tempICM);
}

TestGUIPart2::TestGUIPart2(InteractiveGUI* parent) : WholeScreenGUI(parent->getRemainingDim()) {
	addPart(Global::player->getArmy());
	UnitBuyingMenu* tempUBM = new UnitBuyingMenu(Global::player->getInventory()->getX(), Global::player->getInventory()->getY(),
												 Global::player->getInventory()->getW() * 2, Global::player->getInventory()->getH());
	tempUBM->addUnit(Global::unitHandler->getUnit("Bowman", 1));
	tempUBM->addUnit(Global::unitHandler->getUnit("Bowman", 1));
	tempUBM->addUnit(Global::unitHandler->getUnit("Axeman", 1));
	tempUBM->addUnit(Global::unitHandler->getUnit("Bowman", 1));
	tempUBM->addUnit(Global::unitHandler->getUnit("Bowman", 1));
	tempUBM->addUnit(Global::unitHandler->getUnit("Axeman", 1));
	tempUBM->addUnit(Global::unitHandler->getUnit("Axeman", 1));
	
	addTempPart(tempUBM);
}

TestGUIPart3::TestGUIPart3(InteractiveGUI* parent) : WholeScreenGUI(parent->getRemainingDim()) {
	addPart(Global::player->getArmy());
	Garrison* tempG = new Garrison(Global::player->getArmy()->getX(),
								   parent->getY() + headerSize,
								   Global::player->getArmy()->getW(),
								   Global::player->getArmy()->getY() - parent->getY() - headerSize);
	tempG->getArmy()->addUnit(Global::unitHandler->getUnit("Bowman", 1), UnitAddingPreference::FRONTROWFIRST);
	addTempPart(tempG);
}

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
		int currentItemPrice = getItemPrice(i + currentItemPosition);
		if (currentRenderedItem != NULL) {
			//Rendering the item itself
			SDL_Rect itemDestionationRect = destinationRect;
			itemDestionationRect.x += padding;
			itemDestionationRect.y += padding;
			itemDestionationRect.h = itemSlotHeight - 2 * padding;
			itemDestionationRect.w = itemDestionationRect.h;
			currentRenderedItem->render(itemDestionationRect, true);
			
			//Rendering item name
			ATexture* itemNameText = Global::resourceHandler->getTextTexture(currentRenderedItem->getName(), Global::resourceHandler->colors["whole-header"]);
			Dimension d = itemNameText->getDimensions();
			d *= fontSize;
			d /= Global::defaultFontSize;
			itemDestionationRect.x += padding + itemDestionationRect.w;
			itemDestionationRect.w = d.W();
			itemDestionationRect.h = d.H();
			itemNameText->render(itemDestionationRect);
			
			//Rendering item price
			ATexture* itemPriceText = Global::resourceHandler->getTextTexture(std::to_string(currentItemPrice), Global::resourceHandler->colors["whole-header"]);
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
	if (index < itemsToSell.size()) {
		return itemsToSell[index].first;
	}
	return NULL;
}

int ItemBuyingMenu::getItemPrice(unsigned int index) {
	if (index < itemsToSell.size()) {
		return itemsToSell[index].second;
	}
	return 0;
}

void ItemBuyingMenu::addItem(Item* itemToAdd) {
	itemsToSell.push_back({itemToAdd, 5});
}

void ItemBuyingMenu::removeCurrentItem() {
	itemsToSell.erase(itemsToSell.begin() + selectedItemPosition);
	selectedItemPosition = 0;
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

void ItemBuyingMenu::handleMousePressEvent(int xp, int yp) {
	selectedItemPosition = (yp - y) / itemSlotHeight + currentItemPosition;
	itemInfo->setItem(getItem(selectedItemPosition));
	itemCheckoutMenu->setCurrentItemToBuy(getItem(selectedItemPosition));
	itemCheckoutMenu->setItemToBuyPrice(getItemPrice(selectedItemPosition));
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
	itemToBuyPrice = 0;
	itemToSellPrice = 10;
	buyButton = new Button(x + w - horizontalPadding - rowHeight,
							y + verticalPadding, rowHeight, rowHeight);
	buyButton->setText("Buy");
	sellButton = new Button(x + w - horizontalPadding - rowHeight,
							y + verticalPadding * 5 + rowHeight * 2, rowHeight, rowHeight);
	sellButton->setText("Sell");
	itemBuyingMenu = NULL;
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
	
	//Rendering item to buy
	destinationRect.x += horizontalPadding;
	destinationRect.y += verticalPadding;
	destinationRect.w = rowHeight;
	destinationRect.h = rowHeight;
	if (currentItemToBuy != NULL) {
		currentItemToBuy->render(destinationRect, true);
	}
	
	//Rendering gold icon
	destinationRect.y += 2 * verticalPadding + rowHeight;
	goldText->render(destinationRect);
	
	//Rendering item to sell
	destinationRect.y += 2 * verticalPadding + rowHeight;
	if (currentItemToSell != NULL) {
		currentItemToSell->render(destinationRect, true);
	}
	
	Dimension d;
	//Rendering item price textures
	ATexture* itemPriceTextB;
	if (Global::player->getGold() >= itemToBuyPrice) {
		itemPriceTextB = Global::resourceHandler->getTextTexture(std::to_string(itemToBuyPrice), Global::resourceHandler->colors["unitinfo-values-unchanged"]);
	} else {
		itemPriceTextB = Global::resourceHandler->getTextTexture(std::to_string(itemToBuyPrice), Global::resourceHandler->colors["unitinfo-values-decremented"]);
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
	
	ATexture* goldValueText = Global::resourceHandler->getTextTexture(std::to_string(Global::player->getGold()), Global::resourceHandler->colors["whole-header"]);
	d = goldValueText->getDimensions();
	d *= fontSize;
	d /= Global::defaultFontSize;
	destinationRect.x = x + w / 2 - d.W() / 2;
	destinationRect.y += 2 * verticalPadding + rowHeight;
	destinationRect.w = d.W();
	destinationRect.h = d.H();
	goldValueText->render(destinationRect);
	
	ATexture* itemPriceTextS = Global::resourceHandler->getTextTexture(std::to_string(itemToSellPrice), Global::resourceHandler->colors["unitinfo-values-incremented"]);
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

int ItemCheckoutMenu::getItemToBuyPrice() {
	return itemToBuyPrice;
}

int ItemCheckoutMenu::getItemToSellPrice() {
	return itemToSellPrice;
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

void ItemCheckoutMenu::setItemToBuyPrice(int newItemToBuyPrice) {
	itemToBuyPrice = newItemToBuyPrice;
}

void ItemCheckoutMenu::setItemToSellPrice(int newItemToSellPrice) {
	itemToSellPrice = newItemToSellPrice;
}

void ItemCheckoutMenu::setItemBuyingMenu(ItemBuyingMenu* newItemBuyingMenu) {
	itemBuyingMenu = newItemBuyingMenu;
}

void ItemCheckoutMenu::handleMousePressEvent(int xp, int yp) {
	//Checing button clicking
	//NOTE very ugly
	if (buyButton->contains(xp, yp)) {
		if (currentItemToBuy != NULL) {
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
			Global::player->giveGold(itemToSellPrice);
			itemToSellPrice = 0;
			currentItemToSell = NULL;
		}
		return;
	}
	
	Item* currItemOnCursor = Global::cursor->getItem();
	Global::cursor->setItem(currentItemToSell);
	currentItemToSell = currItemOnCursor;
}

UnitBuyingMenu::UnitBuyingMenu(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	bgText = Global::resourceHandler->getATexture(TT::GUI, "marketcheckoutbg"); // TODO Change this
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
		delete unitsToSell[i].first;
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
		
		unitsToSell[i + currentUnitPosition].first->render(destinationRect);
		
		buyButtons[i]->render();
		
		ATexture* priceText = Global::resourceHandler->getTextTexture(std::to_string(unitsToSell[i + currentUnitPosition].second), Global::resourceHandler->colors["whole-header"]);
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
	if (index < unitsToSell.size()) {
		return unitsToSell[index].first;
	}
	return NULL;
}

int UnitBuyingMenu::getUnitPrice(unsigned int index) {
	if (index < unitsToSell.size()) {
		return unitsToSell[index].second;
	}
	return 0;
}

void UnitBuyingMenu::addUnit(Unit* unitToAdd) {
	unitsToSell.push_back({unitToAdd, 50});
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

void UnitBuyingMenu::handleMousePressEvent(int xp, int yp) {
	for (int i = 0; i < visibleUnitCount; i++) {
		//If we clicked on the buy button
		if (buyButtons[i]->contains(xp, yp)) {
			
			//Price checking
			if (getUnitPrice(currentUnitPosition + i) <= Global::player->getGold()) {
				Unit* unitToAdd = getUnit(currentUnitPosition + i);
				
				//We do not purchase the unit if our army is full
				if (Global::player->getArmy()->addUnit(unitToAdd, UnitAddingPreference::FRONTROWFIRST)) {
					//TODO maybe different unit adding preferences based on unit type
					removeUnit(currentUnitPosition + i);
					Global::player->takeGold(getUnitPrice(currentUnitPosition + i));
				}
			}
			break;
		}
	}
}

void UnitBuyingMenu::handleMouseWheelEvent(bool up) {
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
	garrisonArmy = new Army(x, y, w, h, Global::player->getArmy()->getWidth(), Global::player->getArmy()->getHeight(), true);
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

void Garrison::handleMousePressEvent(int xp, int yp) {
	//Simple passthrough
	garrisonArmy->handleMousePressEvent(xp, yp);
}

void Garrison::handleMouseMotionEvent(int xp, int yp) {
	//Simple passthrough
	garrisonArmy->handleMouseMotionEvent(xp, yp);
}

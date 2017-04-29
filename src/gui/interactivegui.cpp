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
	//addPart(Global::player->getInventory());
	addPart(Global::player->getInventory()->getItemInfo());
	auto temp = new ItemBuyingMenu(Global::player->getInventory()->getDimensionRect());
	temp->addItem(Global::itemHandler->getItem("sword"));
	temp->addItem(Global::itemHandler->getItem("sickle"));
	addTempPart(temp);
}

ItemBuyingMenu::ItemBuyingMenu(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	numberOfDisplayableItems = 5;
	itemSlotHeight = h / numberOfDisplayableItems;
	padding = 1;
	currentItemPosition = 0;
	itemBgText = Global::resourceHandler->getATexture(TT::GUI, "marketitemgb");
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
		itemBgText->render(destinationRect);
		
		//Rendering current item if possible
		Item* currentRenderedItem = getItem(i + currentItemPosition);
		if (currentRenderedItem != NULL) {
			SDL_Rect itemDestionationRect = destinationRect;
			itemDestionationRect.x += padding;
			itemDestionationRect.y += padding;
			itemDestionationRect.h = itemSlotHeight - 2 * padding;
			itemDestionationRect.w = itemDestionationRect.h;
			currentRenderedItem->render(itemDestionationRect, true);
			
			ATexture* itemNameText = Global::resourceHandler->getTextTexture(currentRenderedItem->getName(), Global::resourceHandler->colors["whole-header"]);
			Dimension d = itemNameText->getDimensions();
			d *= 28;
			d /= Global::defaultFontSize;
			itemDestionationRect.x += padding + itemDestionationRect.w;
			itemDestionationRect.w = d.W();
			itemDestionationRect.h = d.H();
			itemNameText->render(itemDestionationRect);
		}
		
		
	}
}

Item* ItemBuyingMenu::getItem(unsigned int index) {
	if (index < itemsToSell.size()) {
		return itemsToSell[index];
	}
	return NULL;
}

void ItemBuyingMenu::addItem(Item* itemToAdd) {
	itemsToSell.push_back(itemToAdd);
}

void ItemBuyingMenu::handleMousePressEvent(int xp, int yp) {
	
}

void ItemBuyingMenu::handleMouseMotionEvent(int xp, int yp) {
	
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

#include "popup.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

Popup::Popup(int xp, int yp, int wp, int hp, PopupType type) : BasicGUI(xp, yp, wp, hp) {
	popupType = type;
	
	padding = 5;
	buttonDimensions = Dimension(150, 50);
	
	//Setting textures
	backgroundT = Global::resourceHandler->getATexture(TT::GUI, "popupback");
	foregroundT = Global::resourceHandler->getATexture(TT::GUI, "popupfore");
	
	text = "";
	textSize = 36;
	
	if (popupType == PopupType::POPUP_OK) {
		buttonOK = new Button(
			x + w - padding - buttonDimensions.W(),
			y + h - padding - buttonDimensions.H(),
			buttonDimensions.W(), buttonDimensions.H()
		);
		buttonOK->setText("OK");
		
		buttonNO = NULL;
		buttonYES = NULL;
	} else if (popupType == PopupType::POPUP_YESNO) {
		buttonNO = new Button(
			x + w - padding * 2 - buttonDimensions.W() * 2,
			y + h - padding - buttonDimensions.H(),
			buttonDimensions.W(), buttonDimensions.H()
		);
		buttonNO->setText("NO");
		buttonYES = new Button(
			x + w - padding - buttonDimensions.W(),
			y + h - padding - buttonDimensions.H(),
			buttonDimensions.W(), buttonDimensions.H()
		);
		buttonYES->setText("YES");
		
		buttonOK = NULL;
	}
}

Popup::Popup(SDL_Rect dimensionRect, PopupType type) : Popup(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h, type) {}

Popup::Popup(int width, int height, PopupType type) : Popup(Global::screenWidth / 2 - width / 2,
															Global::permaGUI->getUpperHeight() + Global::permaGUI->getHeightLeftForMap() / 2 - height / 2,
															width, height, type) {}

Popup::Popup(Dimension dimension, PopupType type) : Popup(dimension.W(), dimension.H(), type) {}

Popup::~Popup() {
	//Deletes the buttons
	if (buttonOK != NULL) {
		delete buttonOK;
	}
	if (buttonYES != NULL) {
		delete buttonYES;
	}
	if (buttonNO != NULL) {
		delete buttonNO;
	}
}

void Popup::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	backgroundT->render(destinationRect);
	destinationRect.x += padding;
	destinationRect.y += padding;
	destinationRect.w -= padding * 2;
	destinationRect.h -= padding * 3 + buttonDimensions.H();
	foregroundT->render(destinationRect);
	
	if (popupType == PopupType::POPUP_OK) {
		buttonOK->render();
	} else if (popupType == PopupType::POPUP_YESNO) {
		buttonNO->render();
		buttonYES->render();
	}
	
	if (text != "") {
		ATexture* textTexture = Global::resourceHandler->getTextTexture(text, Global::resourceHandler->getColor("popup-text"), textSize);
		Dimension d = textTexture->getDimensions();
		//Setting rectangle
		destinationRect.x = x + w / 2 - d.W() / 2;
		destinationRect.y = y + h / 2 - d.H() / 2;
		destinationRect.w = d.W();
		destinationRect.h = d.H();
		textTexture->render(destinationRect);
	} else if (items.size() != 0) {
		ATexture* textTexture = Global::resourceHandler->getTextTexture("Acquired Items:", Global::resourceHandler->getColor("popup-text"), textSize);
		Dimension d = textTexture->getDimensions();
		//Setting rectangle
		destinationRect.x = x + w / 2 - d.W() / 2;
		destinationRect.y = y + padding;
		destinationRect.w = d.W();
		destinationRect.h = d.H();
		textTexture->render(destinationRect);
		
		int lineWidth = w - padding * 2;
		lineWidth /= Global::player->getInventory()->getSlotSize();
		int lineCount = (int)std::ceil((double)items.size() / lineWidth);
		for (unsigned int i = 0; i < items.size(); i++) {
			destinationRect.x = x + (i % lineWidth) * Global::player->getInventory()->getSlotSize();
			destinationRect.y = y + (h - lineCount * Global::player->getInventory()->getSlotSize()) / 2 + ((i / lineWidth) * Global::player->getInventory()->getSlotSize());
			destinationRect.w = Global::player->getInventory()->getSlotSize();
			destinationRect.h = Global::player->getInventory()->getSlotSize();
			items[i]->render(destinationRect, false);
		}
	}
}

PopupType Popup::getPopupType() {
	return popupType;
}

std::string Popup::getText() {
	return text;
}

int Popup::getTextSize() {
	return textSize;
}

Item* Popup::getItem(int index) {
	return items[index];
}

void Popup::setPopupType(PopupType newPopupType) {
	popupType = newPopupType;
}

void Popup::setText(std::string newText) {
	text = newText;
}

void Popup::setTextSize(int newTextSize) {
	textSize = newTextSize;
}

void Popup::addItem(Item* itemToAdd) {
	items.push_back(itemToAdd);
}

void Popup::setItemList(std::vector<Item*> newItemList) {
	items = newItemList;
}

void Popup::handleLeftClickEvent(int xp, int yp) {
	if (popupType == PopupType::POPUP_OK) {
		if (buttonOK->contains(xp, yp)) {
			buttonOK->click();
			Global::guiHandler->clear();
			delete this;
		}
	} else if (popupType == PopupType::POPUP_YESNO) {
		if (buttonYES->contains(xp, yp)) {
			buttonYES->click();
			//delete this;
		}
		if (buttonNO->contains(xp, yp)) {
			buttonNO->click();
			//sdelete this;
		}
	}
}

const Dimension Popup::DEFAULT_DIM = Dimension(800, 400);

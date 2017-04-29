#include "popup.h"

#include "global.h"

Popup::Popup(int xp, int yp, int wp, int hp, PopupType type) : TransientGUI(xp, yp, wp, hp) {
	popupType = type;
	
	padding = 5;
	buttonDimensions = Point(150, 50);
	
	//Setting textures
	backgroundT = Global::resourceHandler->guiTextures["popupback"];
	foregroundT = Global::resourceHandler->guiTextures["popupfore"];
	
	text = "";
	
	if (popupType == PopupType::POPUP_OK) {
		buttonOK = new Button(
			x + w - padding - buttonDimensions.getX(),
			y + h - padding - buttonDimensions.getY(),
			buttonDimensions.getX(), buttonDimensions.getY()
		);
		buttonOK->setText("OK");
		
		buttonNO = NULL;
		buttonYES = NULL;
	} else if (popupType == PopupType::POPUP_YESNO) {
		buttonOK = NULL;
		
		buttonNO = new Button(
			x + w - padding * 2 - buttonDimensions.getX() * 2,
			y + h - padding - buttonDimensions.getY(),
			buttonDimensions.getX(), buttonDimensions.getY()
		);
		buttonNO->setText("NO");
		buttonYES = new Button(
			x + w - padding - buttonDimensions.getX(),
			y + h - padding - buttonDimensions.getY(),
			buttonDimensions.getX(), buttonDimensions.getY()
		);
		buttonYES->setText("YES");
	}
}

Popup::Popup(SDL_Rect dimensionRect, PopupType type) : Popup(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h, type) {}

Popup::Popup(int width, int height, PopupType type) : Popup(Global::screenWidth / 2 - width / 2,
															Global::permaGUI->getUpperHeight() + Global::permaGUI->getHeightLeftForMap() / 2 - height / 2,
															width, height, type) {}

Popup::Popup(Point dimension, PopupType type) : Popup(dimension.getX(), dimension.getY(), type) {}

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
	SDL_RenderCopy(Global::renderer, backgroundT, NULL, &destinationRect);
	destinationRect.x += padding;
	destinationRect.y += padding;
	destinationRect.w -= padding * 2;
	destinationRect.h -= padding * 3 + buttonDimensions.getY();
	SDL_RenderCopy(Global::renderer, foregroundT, NULL, &destinationRect);
	
	if (popupType == PopupType::POPUP_OK) {
		buttonOK->render();
	} else if (popupType == PopupType::POPUP_YESNO) {
		buttonNO->render();
		buttonYES->render();
	}
	
	if (text != "") {
		SDL_Texture* textTexture = Global::resourceHandler->getTextTexture(Text(text, Global::resourceHandler->colors["popup-text"]));
		int tw, th;
		SDL_QueryTexture(textTexture, NULL, NULL, &tw, &th);
		//TODO change this
		int textSize = 36;
		tw = tw / Global::defaultFontSize * textSize;
		th = th / Global::defaultFontSize * textSize;
		//Setting rectangle
		destinationRect.x = x + w / 2 - tw / 2;
		destinationRect.y = y + h / 2 - th / 2;
		destinationRect.w = tw;
		destinationRect.h = th;
		SDL_RenderCopy(Global::renderer, textTexture, NULL, &destinationRect);
	}
}

PopupType Popup::getPopupType() {
	return popupType;
}

std::string Popup::getText() {
	return text;
}

void Popup::setPopupType(PopupType newPopupType) {
	popupType = newPopupType;
}

void Popup::setText(std::string newText) {
	text = newText;
}

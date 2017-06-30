#include "wholescreengui.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

WholeScreenGUI::WholeScreenGUI(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	headerSize = h / 10;
	headerT = Global::resourceHandler->getATexture(TT::GUI, "guiheader");
	headerText = "";
	isRenderableWhenNotOwnedByPlayer = true;
	belongsToBattle = false;
}

WholeScreenGUI::WholeScreenGUI(SDL_Rect dimensionRect) : WholeScreenGUI(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

WholeScreenGUI::~WholeScreenGUI() {
	for (unsigned int i = 0; i < tempParts.size(); i++) {
		delete tempParts[i];
	}
}

void WholeScreenGUI::render() {
	//Rendering header
	SDL_Rect destinationRect;
	destinationRect.x = x;
	destinationRect.y = y;
	destinationRect.w = w;
	destinationRect.h = headerSize;
	headerT->render(destinationRect);
	
	//Rendereing header text
	int headerTextSize = headerSize;
	ATexture* headerTextTexture = Global::resourceHandler->getTextTexture(headerText, Global::resourceHandler->getColor("whole-header"), headerTextSize);
	Dimension d = headerTextTexture->getDimensions();
	destinationRect.x = x + w / 2 - d.W() / 2;
	destinationRect.y = y + headerSize / 2 - d.H() / 2;
	destinationRect.w = d.W();
	destinationRect.h = d.H();
	headerTextTexture->render(destinationRect);
	
	//Render all parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i] != NULL) {
			parts[i]->render();
		}
	}
	
	for (unsigned int i = 0; i < tempParts.size(); i++) {
		if (tempParts[i] != NULL) {
			tempParts[i]->render();
		}
	}
}

void WholeScreenGUI::addPart(BasicGUI* part) {
	parts.push_back(part);
}

void WholeScreenGUI::addTempPart(BasicGUI* part) {
	tempParts.push_back(part);
}


std::string WholeScreenGUI::getHeaderText() {
	return headerText;
}

int WholeScreenGUI::getHeaderSize() {
	return headerSize;
}

bool WholeScreenGUI::getIsRenderableWhenNotOwnedByPlayer() {
	return isRenderableWhenNotOwnedByPlayer;
}

bool WholeScreenGUI::getBelongsToBattle() {
	return belongsToBattle;
}

void WholeScreenGUI::setHeaderText(std::string newHeaderText) {
	headerText = newHeaderText;
}

void WholeScreenGUI::setIsRenderableWhenNotOwnedByPlayer(bool newIsRenderableWhenNotOwnedByPlayer) {
	isRenderableWhenNotOwnedByPlayer = newIsRenderableWhenNotOwnedByPlayer;
}

void WholeScreenGUI::setBelongsToBattle(bool newBelongsToBattle) {
	belongsToBattle = newBelongsToBattle;
}

void WholeScreenGUI::handleLeftClickEvent(int xp, int yp) {
	//Events are passed to parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i]->contains(xp, yp)) {
			parts[i]->handleLeftClickEvent(xp, yp);
			break;
		}
	}
	for (unsigned int i = 0; i < tempParts.size(); i++) {
		if (tempParts[i]->contains(xp, yp)) {
			tempParts[i]->handleLeftClickEvent(xp, yp);
			break;
		}
	}
}

void WholeScreenGUI::handleRightClickEvent(int xp, int yp) {
	//Events are passed to parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i]->contains(xp, yp)) {
			parts[i]->handleRightClickEvent(xp, yp);
			break;
		}
	}
	for (unsigned int i = 0; i < tempParts.size(); i++) {
		if (tempParts[i]->contains(xp, yp)) {
			tempParts[i]->handleRightClickEvent(xp, yp);
			break;
		}
	}
}

void WholeScreenGUI::handleMouseMotionEvent(int xp, int yp) {
	//Events are passed to parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i]->contains(xp, yp)) {
			parts[i]->handleMouseMotionEvent(xp, yp);
			break;
		}
	}
	for (unsigned int i = 0; i < tempParts.size(); i++) {
		if (tempParts[i]->contains(xp, yp)) {
			tempParts[i]->handleMouseMotionEvent(xp, yp);
			break;
		}
	}
}

void WholeScreenGUI::handleMouseWheelEvent(bool up) {
	//Events are passed to parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i]->contains(Global::cursor->getPosition())) {
			parts[i]->handleMouseWheelEvent(up);
			break;
		}
	}
	for (unsigned int i = 0; i < tempParts.size(); i++) {
		if (tempParts[i]->contains(Global::cursor->getPosition())) {
			tempParts[i]->handleMouseWheelEvent(up);
			break;
		}
	}
}

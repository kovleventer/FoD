#include "wholescreengui.h"

#include "../global.h"

/*!
 * @author kovlev
 */

WholeScreenGUI::WholeScreenGUI(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	headerSize = h / 10;
	headerT = Global::resourceHandler->getATexture(TT::GUI, "guiheader");
	headerText = "";
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
	ATexture* headerTextTexture = Global::resourceHandler->getTextTexture(headerText, Global::resourceHandler->colors["whole-header"]);
	Dimension d = headerTextTexture->getDimensions();
	d *= headerTextSize;
	d /= Global::defaultFontSize;
	destinationRect.x = x + w / 2 - d.W() / 2;
	destinationRect.y = y + headerSize / 2 - d.H() / 2;
	destinationRect.w = d.W();
	destinationRect.h = d.H();
	headerTextTexture->render(destinationRect);
	
	//Render all parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		//NOTE NULL-checking is not done here
		parts[i]->render();
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

void WholeScreenGUI::setHeaderText(std::string newHeaderText) {
	headerText = newHeaderText;
}

void WholeScreenGUI::handleMousePressEvent(int xp, int yp) {
	//Events are passed to parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i]->contains(xp, yp)) {
			parts[i]->handleMousePressEvent(xp, yp);
			break;
		}
	}
	for (unsigned int i = 0; i < tempParts.size(); i++) {
		if (tempParts[i]->contains(xp, yp)) {
			tempParts[i]->handleMousePressEvent(xp, yp);
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

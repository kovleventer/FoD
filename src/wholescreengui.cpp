#include "wholescreengui.h"

#include "global.h"

WholeScreenGUI::WholeScreenGUI(int xp, int yp, int wp, int hp) : TransientGUI(xp, yp, wp, hp) {
	headerSize = h / 10;
	headerT = Global::resourceHandler->guiTextures["guiheader"];
	headerText = "";
}

WholeScreenGUI::WholeScreenGUI(SDL_Rect dimensionRect) : WholeScreenGUI(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

WholeScreenGUI::~WholeScreenGUI() {
	for (unsigned int i = 0; i < parts.size(); i++) {
		delete parts[i];
	}
}

void WholeScreenGUI::render() {
	//Rendering header
	SDL_Rect destinationRect;
	destinationRect.x = x;
	destinationRect.y = y;
	destinationRect.w = w;
	destinationRect.h = headerSize;
	SDL_RenderCopy(Global::renderer, headerT, NULL, &destinationRect);
	
	//Rendereing header text
	int headerTextSize = headerSize;
	SDL_Texture* headerTextTexture = Global::resourceHandler->getTextTexture(headerText, Global::resourceHandler->colors["whole-header"]);
	int tw, th;
	SDL_QueryTexture(headerTextTexture, NULL, NULL, &tw, &th);
	tw = tw / Global::defaultFontSize * headerTextSize;
	th = th / Global::defaultFontSize * headerTextSize;
	destinationRect.x = x + w / 2 - tw / 2;
	destinationRect.y = y + headerSize / 2 - th / 2;
	destinationRect.w = tw;
	destinationRect.h = th;
	SDL_RenderCopy(Global::renderer, headerTextTexture, NULL, &destinationRect);
	
	//Render all parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		parts[i]->render();
	}
}

void WholeScreenGUI::addPart(GUIPart* part) {
	parts.push_back(part);
}

std::string WholeScreenGUI::getHeaderText() {
	return headerText;
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
}

void WholeScreenGUI::handleMouseMotionEvent(int xp, int yp) {
	//Events are passed to parts
	for (unsigned int i = 0; i < parts.size(); i++) {
		if (parts[i]->contains(xp, yp)) {
			parts[i]->handleMouseMotionEvent(xp, yp);
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
}

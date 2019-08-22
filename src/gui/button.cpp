#include "button.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

Button::Button() : Button(0, 0, 0, 0) {}

Button::Button(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	texture = Global::resourceHandler->getATexture(TT::GUI, "button");
	functionPointer = NULL;
	text = "";
	fontSize = 16;
}

void Button::render() {
	//Setting the rectangle is pretty easy
	SDL_Rect destinationRect = {x, y, w, h};
	texture->render(destinationRect);
	
	if (text != "") {
		ATexture* textTexture = Global::resourceHandler->getTextTexture(text, Global::resourceHandler->getColor("button-text"), fontSize);
		PivotRender::render(textTexture, Pivot::CENTER, Point(x + w / 2, y + h / 2));
	}
}

std::string Button::getText() {
	return text;
}

int Button::getFontSize() {
	return fontSize;
}

void Button::setText(std::string newText) {
	text = newText;
}

void Button::setFontSize(int newFontSize) {
	fontSize = newFontSize;
}

void Button::click() {
	//Execute the current callback function pointer
	if (functionPointer != NULL)
		(functionPointer)();
}

void Button::setOnClick(eventFunction newOnClick) {
	functionPointer = newOnClick;
}

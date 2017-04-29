#include "button.h"

#include "global.h"

Button::Button(int xp, int yp, int wp, int hp) {
	x = xp;
	y = yp;
	w = wp;
	h = hp;
	texture = Global::resourceHandler->guiTextures["button"];
	functionPointer = NULL;
}

void Button::render() {
	//Setting the rectangle is pretty easy
	SDL_Rect destinationRect = {x, y, w, h};
	SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
}

int Button::getX() {
	return x;
}

int Button::getY() {
	return y;
}

int Button::getW() {
	return w;
}

int Button::getH() {
	return h;
}

void Button::setX(int newX) {
	x = newX;
}

void Button::setY(int newY) {
	y = newY;
}

void Button::setW(int newW) {
	w = newW;
}

void Button::setH(int newH) {
	h = newH;
}

bool Button::isClicked(Point cursorPosition) {
	//Simple is-in-rectangle check
	return cursorPosition.getX() >= x && cursorPosition.getX() <= x + w && cursorPosition.getY() >= y && cursorPosition.getY() <= y + h;
}

void Button::click() {
	//Execute the current callback function pointer
	if (functionPointer != NULL)
		(functionPointer)();
}

void Button::setOnClick(eventFunction newOnClick) {
	functionPointer = newOnClick;
}

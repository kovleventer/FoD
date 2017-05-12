#include "basicgui.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

BasicGUI::BasicGUI(int xp, int yp, int wp, int hp) {
	x = xp;
	y = yp;
	w = wp;
	h = hp;
}

BasicGUI::BasicGUI(SDL_Rect dimensionRect) : BasicGUI(dimensionRect.x, dimensionRect.y, dimensionRect.w, dimensionRect.h) {}

BasicGUI::~BasicGUI() {}

int BasicGUI::getX() {
	return x;
}

int BasicGUI::getY() {
	return y;
}

int BasicGUI::getW() {
	return w;
}

int BasicGUI::getH() {
	return h;
}

SDL_Rect BasicGUI::getDimensionRect() {
	return {x, y, w, h};
}

void BasicGUI::setX(int newX) {
	x = newX;
}

void BasicGUI::setY(int newY) {
	y = newY;
}

void BasicGUI::setW(int newW) {
	w = newW;
}

void BasicGUI::setH(int newH) {
	h = newH;
}

void BasicGUI::setDimensionRect(SDL_Rect newDimensionRect) {
	x = newDimensionRect.x;
	y = newDimensionRect.y;
	w = newDimensionRect.w;
	h = newDimensionRect.h;
}

bool BasicGUI::contains(int xp, int yp) {
	return xp >= x && yp >= y && xp < x + w && yp < y + h;
}

bool BasicGUI::contains(Point p) {
	return contains(p.getX(), p.getY());
}

void BasicGUI::render() {
	//Do not use this rendering
	SDL_Rect destinationRect = {x, y, w, h};
	SDL_RenderFillRect(Global::renderer, &destinationRect);
}

//Do nothing since they are virtual functions
void BasicGUI::handleMousePressEvent(int xp, int yp) {}
void BasicGUI::handleMouseMotionEvent(int xp, int yp) {}
void BasicGUI::handleMouseWheelEvent(bool up) {}

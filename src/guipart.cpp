#include "guipart.h"

#include "global.h"

GUIPart::GUIPart(int xp, int yp, int wp, int hp) {
	x = xp;
	y = yp;
	w = wp;
	h = hp;
}

GUIPart::~GUIPart() {}

int GUIPart::getX() {
	return x;
}

int GUIPart::getY() {
	return y;
}

int GUIPart::getW() {
	return w;
}

int GUIPart::getH() {
	return h;
}

void GUIPart::setX(int newX) {
	x = newX;
}

void GUIPart::setY(int newY) {
	y = newY;
}

void GUIPart::setW(int newW) {
	w = newW;
}

void GUIPart::setH(int newH) {
	h = newH;
}

bool GUIPart::contains(int xp, int yp) {
	return xp >= x && yp >= y && xp < x + w && yp < y + h;
}

bool GUIPart::contains(Point p) {
	return contains(p.getX(), p.getY());
}

void GUIPart::render() {
	//Do not use this rendering
	SDL_Rect destinationRect = {x, y, w, h};
	SDL_RenderFillRect(Global::renderer, &destinationRect);
}

//Do nothing since they are virtual functions
void GUIPart::handleMousePressEvent(int xp, int yp) {}
void GUIPart::handleMouseMotionEvent(int xp, int yp) {}
void GUIPart::handleMouseWheelEvent(bool up) {}

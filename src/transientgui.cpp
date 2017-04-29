#include "transientgui.h"

#include "global.h"

TransientGUI::TransientGUI(int xp, int yp, int wp, int hp) {
	x = xp;
	y = yp;
	w = wp;
	h = hp;
}

TransientGUI::~TransientGUI() {}

int TransientGUI::getX() {
	return x;
}

int TransientGUI::getY() {
	return y;
}

int TransientGUI::getW() {
	return w;
}

int TransientGUI::getH() {
	return h;
}

void TransientGUI::setX(int newX) {
	x = newX;
}

void TransientGUI::setY(int newY) {
	y = newY;
}

void TransientGUI::setW(int newW) {
	w = newW;
}

void TransientGUI::setH(int newH) {
	h = newH;
}

void TransientGUI::render() {
	SDL_Rect destinationRect = {x, y, w, h};
	//Renders a random rect (shouldnt be called directly)
	SDL_RenderFillRect(Global::renderer, &destinationRect);
}

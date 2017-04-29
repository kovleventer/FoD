#include "guihandler.h"

#include "global.h"

GUIHandler::GUIHandler() {
	clear();
}

bool GUIHandler::isLocked() {
	return hasGUI;
}

bool GUIHandler::isHardlocked() {
	return hardlocked;
}

TransientGUI* GUIHandler::getGUI() {
	return currentGUI;
}

Battle* GUIHandler::getBattle() {
	return currentBattle;
}

void GUIHandler::setGUI(TransientGUI* newGUI) {
	currentGUI = newGUI;
	hasGUI = (currentGUI != NULL);
}

void GUIHandler::setBattle(Battle* newBattle) {
	currentBattle = newBattle;
	hardlocked = (currentBattle != NULL);
}

void GUIHandler::clear() {
	currentGUI = NULL;
	currentBattle = NULL;
	hasGUI = false;
	hardlocked = false;
}

void GUIHandler::render() {
	if (hasGUI) {
		//Rendering semi-transparent black rectangle over the map, when we are focusing on a gui
		SDL_Rect destinationRect = {0, Global::permaGUI->getUpperHeight(), Global::screenWidth, Global::permaGUI->getHeightLeftForMap()};
		SDL_SetRenderDrawColor(Global::renderer, 0x00, 0x00, 0x00, 0xAF);
		SDL_RenderFillRect(Global::renderer, &destinationRect);
		currentGUI->render();
	}
}

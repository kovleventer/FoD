#include "guihandler.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

GUIHandler::GUIHandler() {
	hasGUIInQueue = false;
	currentBattle = NULL;
	clear();
}

GUIHandler::~GUIHandler() {
	stdex::clear_ptr_que(popups);
}

bool GUIHandler::isLocked() {
	return hasGUI;
}

bool GUIHandler::isHardlocked() {
	return hardlocked;
}

BasicGUI* GUIHandler::getGUI() {
	return currentGUI;
}

Battle* GUIHandler::getBattle() {
	return currentBattle;
}

void GUIHandler::setGUI(BasicGUI* newGUI) {
	if (dynamic_cast<Popup*>(newGUI) != NULL) {
		std::clog << "Error: Tried to set popup as gui. Please use the addPopup function" << std::endl;
		return;
	}
	
	if (newGUI == NULL) return;
	
	if (hasGUI && !hasGUIInQueue) {
		popups.push(newGUI);
		hasGUIInQueue = true;
	}
	if (!hasGUI) {
		currentGUI = newGUI;
		hasGUI = true;
		
		WholeScreenGUI* possibleBattleWSGUI = dynamic_cast<WholeScreenGUI*>(currentGUI);
		if (possibleBattleWSGUI != NULL && possibleBattleWSGUI->getBelongsToBattle()) {
			std::thread t([this] {
				this->currentBattle->start();
			});
			t.detach();
		}
	}
	
	//Stopping player movement if neccesary
	if (hasGUI && (Global::player->getState() == PlayerState::MOVING || Global::player->getState() == PlayerState::WAITING)) {
		Global::player->clearPath();
	}
}

void GUIHandler::setBattle(Battle* newBattle) {
	currentBattle = newBattle;
	hardlocked = (currentBattle != NULL);
}

void GUIHandler::clear() {
	if (popups.size() == 0) {
		currentGUI = NULL;
		hasGUI = false;
		hardlocked = false;
		hasGUIInQueue = false;
	} else {
		currentGUI = popups.front();
		hasGUI = true;
		if (dynamic_cast<Popup*>(currentGUI) != NULL) {
			hardlocked = true;
		}
		WholeScreenGUI* possibleBattleWSGUI = dynamic_cast<WholeScreenGUI*>(currentGUI);
		if (possibleBattleWSGUI != NULL) {
			if (possibleBattleWSGUI->getBelongsToBattle()) {
				std::thread t([this] {
					this->currentBattle->start();
				});
				t.detach();
			} else {
				hardlocked = false;
			}
		}
		InteractiveGUI* possibleIntGUI = dynamic_cast<InteractiveGUI*>(currentGUI);
		if (possibleIntGUI != NULL) {
			hardlocked = false;
		}
		
		popups.pop();
		//Stopping player movement if neccesary
		if (hasGUI && (Global::player->getState() == PlayerState::MOVING || Global::player->getState() == PlayerState::WAITING)) {
			Global::player->clearPath();
		}
	}
}

void GUIHandler::hardClear() {
	currentGUI = NULL;
	currentBattle = NULL;
	hasGUI = false;
	hardlocked = false;
	hasGUIInQueue = false;
	
	stdex::clear_ptr_que(popups);
}

void GUIHandler::render() {
	if (hasGUI) {
		//Rendering semi-transparent black rectangle over the map, when we are focusing on a gui
		SDL_Rect destinationRect = {0, Global::permaGUI->getUpperHeight(), Global::screenWidth, Global::permaGUI->getHeightLeftForMap()};
		SDL_SetRenderDrawColor(Global::renderer, 0x00, 0x00, 0x00, 0xAF);
		SDL_RenderFillRect(Global::renderer, &destinationRect);
		WholeScreenGUI* possibleBattleWSGUI = dynamic_cast<WholeScreenGUI*>(currentGUI);
		if (possibleBattleWSGUI != NULL && possibleBattleWSGUI->getBelongsToBattle()) {
			currentBattle->render();
		} else {
			currentGUI->render();
		}
	}
}

void GUIHandler::addPopup(Popup* popupToAdd) {
	if (popupToAdd == NULL) return;
	popups.push(popupToAdd);
	if (!hasGUI) {
		currentGUI = popups.front();
		hasGUI = true;
		if (dynamic_cast<Popup*>(currentGUI) == NULL) {
			hardlocked = false;
			hasGUIInQueue = false;
		} else {
			hardlocked = true;
		}
		popups.pop();
		//Stopping player movement if neccesary
		if (hasGUI && (Global::player->getState() == PlayerState::MOVING || Global::player->getState() == PlayerState::WAITING)) {
			Global::player->clearPath();
		}
	}
}

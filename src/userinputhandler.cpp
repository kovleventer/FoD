#include "userinputhandler.h"

#include "global.h"
#include "player.h"
#include "pathfinding.h"
#include "tile.h"
#include "game.h"

void UserInputHandler::handleKeyPressEvent(SDL_Event e) {
	
}

void UserInputHandler::handleKeyDownEvent(const Uint8* keyboardState) {
	//If the user presses escape we quit
	if (keyboardState[SDL_SCANCODE_ESCAPE]) {
		Game::quit();
		return;
	}
	
	if (Global::guiHandler->isLocked()) {
		return;
	}
	
	if (Global::player->getState() == PlayerState::MOVING) {
		return;
	}
	
	int moveVectorX = 0;
	int moveVectorY = 0;
	if (keyboardState[SDL_SCANCODE_UP]) { //UP
		moveVectorY -= Global::camera->getSpeed();
	}
	if (keyboardState[SDL_SCANCODE_DOWN]) { //DOWN
		moveVectorY += Global::camera->getSpeed();
	}
	if (keyboardState[SDL_SCANCODE_LEFT]) { //LEFT
		moveVectorX -= Global::camera->getSpeed();
	}
	if (keyboardState[SDL_SCANCODE_RIGHT]) { //RIGHT
		moveVectorX += Global::camera->getSpeed();
	}
	
	
	Global::camera->move(moveVectorX, moveVectorY);
}

void UserInputHandler::handleMousePressEvent(SDL_Event e) {
	if (e.button.button == SDL_BUTTON_LEFT) {
		if (Global::cursor->getPosition().getY() < Global::permaGUI->getUpperHeight() || Global::cursor->getPosition().getY() > Global::screenHeight - Global::permaGUI->getLowerHeight()) {
			if (!Global::guiHandler->isHardlocked()) {
				if (Global::cursor->getItem() == NULL) {
					for (unsigned int i = 0; i < Global::permaGUI->getButtons().size(); i++) {
						if (Global::permaGUI->getButtons()[i]->isClicked(Global::cursor->getPosition())) {
							Global::permaGUI->getButtons()[i]->click();
						}
					}
					return;
				}
			}
		}
		
		if (Global::guiHandler->isLocked()) {
			WholeScreenGUI* possibleGUI = dynamic_cast<WholeScreenGUI*>(Global::guiHandler->getGUI());
			if (possibleGUI == NULL) {
				Popup* popup = (Popup*)Global::guiHandler->getGUI();
				if (popup->getPopupType() == PopupType::POPUP_OK) {
					if (popup->buttonOK->isClicked(Global::cursor->getPosition())) {
						popup->buttonOK->click();
					}
				} else if (popup->getPopupType() == PopupType::POPUP_YESNO) {
					if (popup->buttonYES->isClicked(Global::cursor->getPosition())) {
						popup->buttonYES->click();
					}
					if (popup->buttonNO->isClicked(Global::cursor->getPosition())) {
						popup->buttonNO->click();
					}
				}
			} else {
				possibleGUI->handleMousePressEvent(Global::cursor->getPosition().getX(), Global::cursor->getPosition().getY());
			}
			return;
		}
		
		switch (Global::player->getState()) {
			case PlayerState::STANDING: {
				Point cursorTilePos = Global::map->getTileFromCursorPosition(Global::cursor->getPosition());
				//If something is buggy
				if (Global::map->getTile(cursorTilePos) == NULL) {
					return;
				}
				
				//We dont calculate paths when we click on the player
				if (cursorTilePos == Global::player->getPosition()) {
					return;
				}
				
				if (Global::map->getTile(cursorTilePos)->getTileInfo() != TileInfo::IMPASSABLE) {
					
					Global::player->setFollow(Global::map->getNPCOnTile(cursorTilePos));
					
					Global::player->setPath(Pathfinding::findPath(Global::player->getPosition(), cursorTilePos));
					Global::player->setHasPlannedPath(true);
					Global::player->setState(PlayerState::HAS_WAY);
				}
				break;
			}
			case PlayerState::HAS_WAY:
				if (Global::player->getPath()[Global::player->getPath().size()-1] == Global::map->getTileFromCursorPosition(Global::cursor->getPosition())) {
					Global::player->setState(PlayerState::MOVING);
				} else {
					Global::player->setHasPlannedPath(false);
					Global::player->setState(PlayerState::STANDING);
					Global::player->setFollow(NULL);
				}
				break;
			case PlayerState::MOVING:
				Global::player->setState(PlayerState::STANDING);
				Global::player->setProgressVector(Point());
				Global::player->setTileProgress(0);
				Global::player->clearPath();
				break;
		}
	}
}

void UserInputHandler::handleMouseReleaseEvent(SDL_Event e) {
	
}

void UserInputHandler::handleMouseMotionEvent(int x, int y) {
	if (Global::guiHandler->isLocked()) {
		WholeScreenGUI* possibleGUI = dynamic_cast<WholeScreenGUI*>(Global::guiHandler->getGUI());
		if (possibleGUI == NULL) {
			//TODO Popup stuff
		} else {
			Global::player->getArmy()->clearHovering();
			possibleGUI->handleMouseMotionEvent(x, y);
		}
		
		return;
	}
	
	if (Global::player->getState() == PlayerState::MOVING) {
		return;
	}
	
	int moveVectorX = 0;
	int moveVectorY = 0;
	if (y == 0) { //UP
		moveVectorY -= Global::camera->getSpeed();
	}
	if (y == Global::screenHeight - 1) { //DOWN
		moveVectorY += Global::camera->getSpeed();
	}
	if (x == 0) { //LEFT
		moveVectorX -= Global::camera->getSpeed();
	}
	if (x == Global::screenWidth - 1) { //RIGHT
		moveVectorX += Global::camera->getSpeed();
	}
	
	Global::camera->move(moveVectorX, moveVectorY);
}

void UserInputHandler::handleMouseWheelEvent(bool up) {
	if (Global::guiHandler->isLocked()) {
		WholeScreenGUI* possibleGUI = dynamic_cast<WholeScreenGUI*>(Global::guiHandler->getGUI());
		if (possibleGUI == NULL) {
			//TODO Popup stuff
		} else {
			possibleGUI->handleMouseWheelEvent(up);
		}
		return;
	}
}

#include "userinputhandler.h"

#include "global.h"
#include "../map/player.h"
#include "../util/pathfinding.h"
#include "../map/tile.h"
#include "game.h"

/*!
 * @author kovlev
 */

void UserInputHandler::handleKeyPressEvent(SDL_Event e) {
	//Useful:
	//https://wiki.libsdl.org/SDL_Keycode
	switch (e.key.keysym.sym) {
		case SDLK_F5:
			//Press F5 for displaying game ticks
			Global::permaGUI->setRenderDebugTickInfo(!Global::permaGUI->getRenderDebugTickInfo());
			break;
		case SDLK_F6:
			//Press F6 for debug info on map passability
			Global::map->setAllowDebug(!Global::map->getAllowDebug());
			break;
		case SDLK_F7:
			//Press F7 for freeing some memory
			Global::resourceHandler->clearTextTextures();
			break;
		case SDLK_F8:
			//Press F8 for showing tile coordinates on tiles
			Global::map->setAllowCoordinateRendering(!Global::map->getAllowCoordinateRendering());
			break;
		case SDLK_ESCAPE:
			//If the user presses escape we quit
			Game::quit();
			break;
	}
}

void UserInputHandler::handleKeyDownEvent(const Uint8* keyboardState) {
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
		//Checking permaGUI buttons
		if (Global::cursor->getPosition().getY() < Global::permaGUI->getUpperHeight() || Global::cursor->getPosition().getY() > Global::screenHeight - Global::permaGUI->getLowerHeight()) {
			if (!Global::guiHandler->isHardlocked()) {
				if (Global::cursor->getItem() == NULL) {
					for (unsigned int i = 0; i < Global::permaGUI->getButtons().size(); i++) {
						if (Global::permaGUI->getButtons()[i]->contains(Global::cursor->getPosition())) {
							Global::permaGUI->getButtons()[i]->click();
						}
					}
					return;
				}
			}
		}
		
		//Checking present gui elements
		if (Global::guiHandler->isLocked()) {
			Global::guiHandler->getGUI()->handleLeftClickEvent(Global::cursor->getPosition().getX(), Global::cursor->getPosition().getY());
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
			case PlayerState::BATTLING:
				//Do nothing
				break;
		}
	} else if (e.button.button == SDL_BUTTON_RIGHT) {
		//Checking present gui elements
		if (Global::guiHandler->isLocked()) {
			Global::guiHandler->getGUI()->handleRightClickEvent(Global::cursor->getPosition().getX(), Global::cursor->getPosition().getY());
			return;
		}
	}
}

void UserInputHandler::handleMouseReleaseEvent(SDL_Event e) {}

void UserInputHandler::handleMouseMotionEvent(int x, int y) {
	//Checking present gui elements
	if (Global::guiHandler->isLocked()) {
		Global::guiHandler->getGUI()->handleMouseMotionEvent(Global::cursor->getPosition().getX(), Global::cursor->getPosition().getY());
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
		
		Global::guiHandler->getGUI()->handleMouseWheelEvent(up);
		
		return;
	}
	
	//Immersive zooming
	if (up) {
		if (Global::tileSize <= 256) {
			Global::tileSize *= 2;
		}
	} else {
		if (Global::tileSize >= 4) {
			Global::tileSize /= 2;
		}
	}
}

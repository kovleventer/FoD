#include "cursor.h"

#include "global.h"
#include "tile.h"
#include "userinputhandler.h"

Cursor::Cursor(std::string text) {
	//To make sure that the texture is never NULL
	texture = Global::resourceHandler->cursorTextures[text];
	init();
}

Cursor::Cursor(SDL_Texture* text) {
	texture = text;
	init();
}

void Cursor::render() {
	int w, h;
	//Gets the texture's dimensions
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	
	//Setting rectangle
	SDL_Rect destinationRect;
	destinationRect.x = position.getX();
	destinationRect.y = position.getY();
	if (itemOnCursor == NULL) {
		destinationRect.w = w;
		destinationRect.h = h;
	} else {
		destinationRect.w = Global::player->getInventory()->getSlotSize();
		destinationRect.h = Global::player->getInventory()->getSlotSize();
	}
	SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
}

void Cursor::update() {
	int x, y;
	//Gets the current mouse position
	SDL_GetMouseState(&x, &y);
	
	//We handle mouse motion event every frame, even when the cursor stands still
	UserInputHandler::handleMouseMotionEvent(x, y);
	
	//Sets the mouse position
	position = Point(x, y);
	
	if (itemOnCursor != NULL) {
		texture = itemOnCursor->texture;
		return;
	}
	
	if (Global::guiHandler->isLocked()) {
		texture = Global::resourceHandler->cursorTextures["gui"];
		return;
	}
	//NOTE a bit clunky
	if (y < Global::permaGUI->getUpperHeight() || y > Global::screenHeight - Global::permaGUI->getLowerHeight()) {
		//If we are hovering over the gui
		texture = Global::resourceHandler->cursorTextures["gui"];
	} else {
		if (Global::map->getTile(Global::map->getTileFromCursorPosition(position)) == NULL) {
			//Error handling on degenerate cases
			texture = Global::resourceHandler->cursorTextures["impassable"];
		} else {
			Tile* currentTile = Global::map->getTile(Global::map->getTileFromCursorPosition(position));
			//NOTE we should use switch-case
			if (currentTile->getTileInfo() == TileInfo::FREE) {
				if (Global::map->getNPCOnTile(currentTile) == NULL) {
					texture = Global::resourceHandler->cursorTextures["move"];
				} else {
					//If the tile contains any npcs
					//NOTE it does not check wheter it is friendy or foe
					texture = Global::resourceHandler->cursorTextures["npc"];
				}
			} else if (currentTile->getTileInfo() == TileInfo::IMPASSABLE) {
				//If tile is not passable
				texture = Global::resourceHandler->cursorTextures["impassable"];
			} else {
				//If the tile has a pointer to a friendly structure
				texture = Global::resourceHandler->cursorTextures["friendly"];
			}
		}
	}
}

Point Cursor::getPosition() {
	return position;
}

Item* Cursor::getItem() {
	return itemOnCursor;
}

void Cursor::setItem(Item* newItem) {
	itemOnCursor = newItem;
}

void Cursor::init() {
	position = Point(0, 0);
	itemOnCursor = NULL;
}

#include "cursor.h"

#include "global.h"
#include "tile.h"
#include "userinputhandler.h"

Cursor::Cursor(std::string text) {
	//To make sure that the texture is never NULL
	texture = Global::resourceHandler->getATexture(TT::CURSOR, text);
	init();
}

Cursor::Cursor(ATexture* text) {
	texture = text;
	init();
}

void Cursor::render() {
	//Gets texture size
	Dimension d = texture->getDimensions();
	
	//Setting rectangle
	SDL_Rect destinationRect;
	destinationRect.x = position.getX();
	destinationRect.y = position.getY();
	if (itemOnCursor == NULL) {
		destinationRect.w = d.W();
		destinationRect.h = d.H();
	} else {
		destinationRect.w = Global::player->getInventory()->getSlotSize();
		destinationRect.h = Global::player->getInventory()->getSlotSize();
	}
	texture->render(destinationRect);
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
		texture = Global::resourceHandler->getATexture(TT::CURSOR, "gui");
		return;
	}
	//NOTE a bit clunky
	if (y < Global::permaGUI->getUpperHeight() || y > Global::screenHeight - Global::permaGUI->getLowerHeight()) {
		//If we are hovering over the gui
		texture = Global::resourceHandler->getATexture(TT::CURSOR, "gui");
	} else {
		if (Global::map->getTile(Global::map->getTileFromCursorPosition(position)) == NULL) {
			//Error handling on degenerate cases
			texture = Global::resourceHandler->getATexture(TT::CURSOR, "impassable");
		} else {
			Tile* currentTile = Global::map->getTile(Global::map->getTileFromCursorPosition(position));
			//NOTE we should use switch-case
			if (currentTile->getTileInfo() == TileInfo::FREE) {
				if (Global::map->getNPCOnTile(currentTile) == NULL) {
					texture = Global::resourceHandler->getATexture(TT::CURSOR, "move");
				} else {
					//If the tile contains any npcs
					//NOTE it does not check wheter it is friendly or foe
					texture = Global::resourceHandler->getATexture(TT::CURSOR, "npc");
				}
			} else if (currentTile->getTileInfo() == TileInfo::IMPASSABLE) {
				//If tile is not passable
				texture = Global::resourceHandler->getATexture(TT::CURSOR, "impassable");
			} else {
				//If the tile has a pointer to a friendly structure
				texture = Global::resourceHandler->getATexture(TT::CURSOR, "friendly");
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

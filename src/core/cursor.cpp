#include "cursor.h"

#include "global.h"
#include "../map/tile.h"
#include "userinputhandler.h"

/*!
 * @author kovlev
 */

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
			if (currentTile->getTileInfo() == TileInfo::FREE) {
				NPC* currentNPC = Global::map->getNPCOnTile(currentTile);
				if (currentNPC == NULL) {
					texture = Global::resourceHandler->getATexture(TT::CURSOR, "move");
				} else {
					//If the tile contains any npcs
					if (currentNPC->isEnemy()) {
						texture = Global::resourceHandler->getATexture(TT::CURSOR, "enemy");
					} else {
						texture = Global::resourceHandler->getATexture(TT::CURSOR, "npc");
					}
				}
			} else if (currentTile->getTileInfo() == TileInfo::IMPASSABLE) {
				//If tile is not passable
				texture = Global::resourceHandler->getATexture(TT::CURSOR, "impassable");
			} else if (currentTile->getTileInfo() == TileInfo::INTERACTIVE) {
				//If the tile has a pointer to a structure
				InteractiveWorldObject* currentInteractive = Global::map->getInteractiveOnTile(currentTile);
				NPC* currentInteractiveOwner = dynamic_cast<NPC*>(currentInteractive->getOwner());
				if (currentInteractiveOwner == NULL) {
					texture = Global::resourceHandler->getATexture(TT::CURSOR, "friendly");
				} else {
					if (currentInteractiveOwner->isEnemy()) {
						texture = Global::resourceHandler->getATexture(TT::CURSOR, "enemy");
					} else {
						texture = Global::resourceHandler->getATexture(TT::CURSOR, "friendly");
					}
				}
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

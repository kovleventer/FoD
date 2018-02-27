#include "map.h"

#include "../core/global.h"
#include "../util/path.h"

/*!
 * @author kovlev
 */

Map::Map() {
	tileMapPath = "data/map/tilemap.data";
	tileMapCodesPath = "data/map/tilemapcodes.data";
	
	loadTileMap();
	
	//NOTE ugly solution but i could not come up with a better idea
	getTile(Global::player->getPosition())->entities.push_back(Global::player);
	
	allowDebug = false;
	allowCoordinateRendering = false;
	
	displayInfo = NULL;
	displayInfoPos = Point::INVALID;
	displayDim = Dimension(500, 200);
}

Map::~Map() {
	//delete tilemap
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			delete tiles[i][j];
		}
	}
	
	for (int i = 0; i < width; i++) {
		delete[] tiles[i];
	}
	
	delete[] tiles;
}

int Map::getWidth() {
	return width;
}

int Map::getHeight() {
	return height;
}

int Map::getSize() {
	return size;
}

Tile* Map::getTile(int x, int y) {
	//Index checking
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return NULL;
	}
	return tiles[x][y];
}

Tile* Map::getTile(Point pos) {
	return getTile(pos.getX(), pos.getY());
}

Army* Map::getDisplayInfo() {
	return displayInfo;
}

Point Map::getDisplayInfoPos() {
	return displayInfoPos;
}

void Map::setDisplayInfo(Army* newDisplayInfo) {
	displayInfo = newDisplayInfo;
}

void Map::setDisplayInfoPos(Point newDisplayInfoPos) {
	displayInfoPos = newDisplayInfoPos;
}

void Map::render() {
	//We can set the rendering order here
	
	//Render every layer
	//It does not render the whole map
	renderTileMap();
	renderMapEntities();
	
	//We only render the player's planned path, if we have one
	if (Global::player->getHasPlannedPath()) {
		renderPath();
	}
	
	//DEBUG stuff
	if (allowDebug) {
		renderPassabilityDebugInfo();
	}
	
	if (allowCoordinateRendering) {
		renderTileCoordinates();
	}
	
	//Army info if struct or npc is right clicked
	if (displayInfo != NULL) {
		renderDisplayInfo();
	}
}

Point Map::getTileFromCursorPosition(Point cursorPosition) {
	//If we pass an invalid cursor position we return Point::INVALID
	if (cursorPosition.getX() < 0 ||
		cursorPosition.getX() >= Global::screenWidth ||
		cursorPosition.getY() < 0 ||
		cursorPosition.getY() >= Global::screenHeight) {
		return Point::INVALID;
	}
	
	Point positionOnFullPixmap = Global::camera->getPosition() + cursorPosition;
	
	return positionOnFullPixmap / Global::tileSize;
}

NPC* Map::getNPCOnTile(int x, int y) {
	return getNPCOnTile(Point(x, y));
}

NPC* Map::getNPCOnTile(Point tilePos) {
	if (getTile(tilePos) != NULL) {
		return getNPCOnTile(getTile(tilePos));
	}
	return NULL;
}

NPC* Map::getNPCOnTile(Tile* tile) {
	//We travesrse through the entities list and return the first npc item
	for (unsigned int i = 0; i < tile->entities.size(); i++) {
		NPC* possibleNPC = dynamic_cast<NPC*>(tile->entities[i]);
		if (possibleNPC != NULL) {
			return possibleNPC;
		}
	}
	return NULL;
}

InteractiveWorldObject* Map::getInteractiveOnTile(int x, int y) {
	return getInteractiveOnTile(Point(x, y));
}

InteractiveWorldObject* Map::getInteractiveOnTile(Point tilePos) {
	if (getTile(tilePos) != NULL) {
		return getInteractiveOnTile(getTile(tilePos));
	}
	return NULL;
}

InteractiveWorldObject* Map::getInteractiveOnTile(Tile* tile) {
	return tile->getInterOnTile();
}

bool Map::getAllowDebug() {
	return allowDebug;
}

void Map::setAllowDebug(bool newAllowDebug) {
	allowDebug = newAllowDebug;
}

bool Map::getAllowCoordinateRendering() {
	return allowCoordinateRendering;
}

void Map::setAllowCoordinateRendering(bool newAllowCoordinateRendering) {
	allowCoordinateRendering = newAllowCoordinateRendering;
}

void Map::renderTileMap() {
	//Needed becase threading
	Point camPos = Global::camera->getPosition();
	
	//We only render visible tiles
	Point startTileCoordinates = Point(camPos.getX() / Global::tileSize, camPos.getY() / Global::tileSize);
	Point endTileCoordinates = Point((camPos.getX() + Global::screenWidth) / Global::tileSize + 1, (camPos.getY() + Global::screenHeight) / Global::tileSize + 1);
	
	
	
	for (int i = startTileCoordinates.getX(); i <= endTileCoordinates.getX(); i++) {
		for (int j = startTileCoordinates.getY(); j <= endTileCoordinates.getY(); j++) {
			//If Tile does not exist, we do not render it
			//NOTE this should be unnecessary later since we will fix the camera position
			if (getTile(i, j) == NULL) {
				continue;
			}
			
			//Setting rectangle
			SDL_Rect destinationRect;
			destinationRect.x = i * Global::tileSize - camPos.getX();
			destinationRect.y = j * Global::tileSize - camPos.getY();
			//tileSize is a double now
			destinationRect.w = std::ceil(Global::tileSize);
			destinationRect.h = std::ceil(Global::tileSize);
			getTile(i, j)->texture->render(destinationRect);
		}
	}
}

void Map::renderMapEntities() {
	//Needed becase threading
	Point camPos = Global::camera->getPosition();
	
	//We only render obects on visible tiles
	Point startTileCoordinates = Point(camPos.getX() / Global::tileSize, camPos.getY() / Global::tileSize);
	Point endTileCoordinates = Point((camPos.getX() + Global::screenWidth) / Global::tileSize + 1, (camPos.getY() + Global::screenHeight) / Global::tileSize + 1);
	
	for (int stage = 2; stage <= 3; stage++) {
		for (int i = startTileCoordinates.getY(); i <= endTileCoordinates.getY(); i++) {
			for (int j = startTileCoordinates.getX(); j <= endTileCoordinates.getX(); j++) {
				Tile* current = getTile(j, i);
				
				//If Tile does not exist, we do not render it
				//NOTE this should be unnecessary later since we will fix the camera position
				if (current == NULL) {
					continue;
				}
				if (stage == 2) {
					for (unsigned int k = 0; k < current->backgroundEntities.size(); k++) {
						//currEnt get it?:D
						MapEntity* currEnt = current->backgroundEntities[k];
						
						//NULL-checking
						if (currEnt == NULL) {
							continue;
						}
						
						Dimension d = currEnt->texture->getDimensions();
						
						//Setting rectangle
						SDL_Rect destinationRect;
						
						//We are setting the width and the height first
						//Ugly casting becasue scale is a double member
						destinationRect.w = (int)(((double)d.W() * Global::tileSize / 64) * currEnt->getScale());
						destinationRect.h = (int)(((double)d.H() * Global::tileSize / 64) * currEnt->getScale());
						
						//Every rectangle looks like this:
						// 123
						// 4 5
						// 678
						//By default we are fixing the '7' points (down-center)
						//So the texture's down-center point will be at the same position as its tile's
						
						//The top-left corner of its tile
						destinationRect.x = currEnt->getPosition().getX() * Global::tileSize - camPos.getX();
						destinationRect.y = currEnt->getPosition().getY() * Global::tileSize - camPos.getY();
						
						//We need to move this point with a vector
						destinationRect.x -= (destinationRect.w - Global::tileSize) / 2;
						destinationRect.y -= destinationRect.h - Global::tileSize;
						
						currEnt->texture->render(destinationRect);
					}
				} else {
					for (unsigned int k = 0; k < current->entities.size(); k++) {
						//currEnt get it?:D
						MapEntity* currEnt = current->entities[k];
						
						//NULL-checking
						if (currEnt == NULL) {
							continue;
						}
						
						Dimension d = currEnt->texture->getDimensions();
						
						//Setting rectangle
						SDL_Rect destinationRect;
						
						//We are setting the width and the height first
						//Ugly casting becasue scale is a double member
						destinationRect.w = (int)(((double)d.W() * Global::tileSize / 64) * currEnt->getScale());
						destinationRect.h = (int)(((double)d.H() * Global::tileSize / 64) * currEnt->getScale());
						
						//Every rectangle looks like this:
						// 123
						// 4 5
						// 678
						//By default we are fixing the '7' points (down-center)
						//So the texture's down-center point will be at the same position as its tile's
						
						//The top-left corner of its tile
						destinationRect.x = currEnt->getPosition().getX() * Global::tileSize - camPos.getX();
						destinationRect.y = currEnt->getPosition().getY() * Global::tileSize - camPos.getY();
						
						//We need to move this point with a vector
						destinationRect.x -= (destinationRect.w - Global::tileSize) / 2;
						destinationRect.y -= destinationRect.h - Global::tileSize;
						
						//If the entity is an npc or a player, we add the progressvector
						if (currEnt == Global::player) {
							destinationRect.x += Global::player->getProgressVector().getX() * Global::tileSize;
							destinationRect.y += Global::player->getProgressVector().getY() * Global::tileSize;
						}
						NPC* possibleNPC = dynamic_cast<NPC*>(currEnt);
						if (possibleNPC != NULL && possibleNPC->getMovementType() != NPCMovement::STANDING) {
							if (possibleNPC->getPath() == NULL) continue; //Ugly threading strikes again
							PointD corrigation = (possibleNPC->getPath()->current() - possibleNPC->getPosition()) * Global::tileSize;
							
							destinationRect.x += possibleNPC->getProgressVector().getX() * Global::tileSize;
							destinationRect.x += corrigation.getX();
							destinationRect.y += possibleNPC->getProgressVector().getY() * Global::tileSize;
							destinationRect.y += corrigation.getY();
						}
						
						currEnt->texture->render(destinationRect);
					}
				}
			}
		}
	}
}

void Map::renderPath() {
	SDL_Rect destinationRect;
	destinationRect.w = Global::tileSize;
	destinationRect.h = Global::tileSize;
	//We render only the remaining part of the path
	for (unsigned int i = Global::player->getTileProgress() == 0 ? 1 : Global::player->getTileProgress() + 1; i < Global::player->getPath().size()-1; i++) {
		
		Point thisPoint = Global::player->getPath()[i];
		Point nextPoint = Global::player->getPath()[i + 1];
		Point diff = nextPoint - thisPoint;
		
		//We do not render the path over interactive objects
		if (getTile(thisPoint)->getTileInfo() != TileInfo::FREE) continue;
		
		ATexture* texture;
		
		if (diff == Point(1, -1)) {
			//Up right
			texture = Global::resourceHandler->getATexture(TT::PATH, "upright");
		} else if (diff == Point(1, 0)) {
			//Right
			texture = Global::resourceHandler->getATexture(TT::PATH, "right");
		} else if (diff == Point(1, 1)) {
			//Down right
			texture = Global::resourceHandler->getATexture(TT::PATH, "downright");
		} else if (diff == Point(0, 1)) {
			//Down
			texture = Global::resourceHandler->getATexture(TT::PATH, "down");
		} else if (diff == Point(-1, 1)) {
			//Down left
			texture = Global::resourceHandler->getATexture(TT::PATH, "downleft");
		} else if (diff == Point(-1, 0)) {
			//Left
			texture = Global::resourceHandler->getATexture(TT::PATH, "left");
		} else if (diff == Point(-1, -1)) {
			//Up left
			texture = Global::resourceHandler->getATexture(TT::PATH, "upleft");
		} else if (diff == Point(0, -1)) {
			//Up
			texture = Global::resourceHandler->getATexture(TT::PATH, "up");
		} else {
			std::clog << "Warning: Path rendering bug" << std::endl;
			return;
		}
		
		//Setting renctangle
		destinationRect.x = thisPoint.getX() * Global::tileSize - Global::camera->getPosition().getX();
		destinationRect.y = thisPoint.getY() * Global::tileSize - Global::camera->getPosition().getY();
		texture->render(destinationRect);
	}
	
	//Destination tile
	destinationRect.x = Global::player->getPath()[Global::player->getPath().size() - 1].getX() * Global::tileSize - Global::camera->getPosition().getX();
	destinationRect.y = Global::player->getPath()[Global::player->getPath().size() - 1].getY() * Global::tileSize - Global::camera->getPosition().getY();
	Global::resourceHandler->getATexture(TT::PATH, "destination")->render(destinationRect);
}

void Map::renderDisplayInfo() {
	//Setting rectangle
	SDL_Rect destinationRect;
	destinationRect.x = displayInfoPos.getX() * Global::tileSize - Global::camera->getPosition().getX() + Global::tileSize / 2 - displayDim.W() / 2;
	destinationRect.y = displayInfoPos.getY() * Global::tileSize - Global::camera->getPosition().getY() - displayDim.H();
	destinationRect.w = displayDim.W();
	destinationRect.h = displayDim.H();
	
	//Correcting destinationRect if needed
	if (displayInfoPos.getX() * Global::tileSize + Global::tileSize / 2 - displayDim.W() / 2 < 0) {
		destinationRect.x -= displayInfoPos.getX() * Global::tileSize + Global::tileSize / 2 - displayDim.W() / 2;
	}
	if (displayInfoPos.getX() * Global::tileSize + Global::tileSize / 2 - displayDim.W() / 2 + displayDim.W() > Global::screenWidth) {
		destinationRect.x -= displayInfoPos.getX() * Global::tileSize + Global::tileSize / 2 - displayDim.W() / 2 + displayDim.W() - Global::screenWidth;
	}
	if (displayInfoPos.getY() * Global::tileSize - displayDim.H() < 0) {
		destinationRect.y -= displayInfoPos.getY() * Global::tileSize - displayDim.H();
	}
	
	
	for (int i = 0; i < displayInfo->getWidth(); i++) {
		for (int j = 0; j < displayInfo->getHeight(); j++) {
			Unit* currentUnit = displayInfo->getUnit(i, j);
			SDL_Rect unitRect = destinationRect;
			unitRect.x += i * 100;
			unitRect.y += j * 100;
			unitRect.w = 100;
			unitRect.h = 100;
			if (currentUnit != NULL && !currentUnit->isDead()) {
				currentUnit->render(unitRect);
			} else {
				switch (displayInfo->getUPFromPos(i, j)) {
					case UnitPosition::FRONTROW:
						Global::resourceHandler->getATexture(TT::GUI, "armyslotbg_front")->render(unitRect);
						break;
					case UnitPosition::BACKROW:
						Global::resourceHandler->getATexture(TT::GUI, "armyslotbg_back")->render(unitRect);
						break;
					case UnitPosition::SUPPORT:
						Global::resourceHandler->getATexture(TT::GUI, "armyslotbg_support")->render(unitRect);
						break;
				}
			}
		}
	}
}

void Map::loadTileMap() {
	std::fstream file;
	
	//Loads the codes for the tilemap
	std::map<int, std::string> tileMapCodes;
	file.open(tileMapCodesPath, std::ios::in);
	file >> width;
	file >> height;
	
	//We load the maps dimensions first
	Global::gameBoardWidth = width;
	Global::gameBoardHeight = height;
	size = width * height;
	
	//Generating the tile arrays
	tiles = new Tile**[width];
	for (int i = 0; i < width; ++i) {
		tiles[i] = new Tile*[height];
	}
	
	
	std::string isImpassable;
	int tileCode;
	std::string tileName;
	while (file >> isImpassable && file >> tileCode && file >> std::quoted(tileName)) {
		tileMapCodes[tileCode] = tileName;
		if (isImpassable == "x" || isImpassable == "X") {
			baseImpassable.push_back(tileName);
		}
	}
	file.close();
	
	//Loads the actual tilemap
	file.open(tileMapPath, std::ios::in);
	
	//Initializing the contents of the 2D array
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int tempType;
			file >> tempType;
			tiles[j][i] = new Tile(tileMapCodes[tempType]);
		}
	}
	if (!file) {
		//std::clog << "Warning: Map might be corrupted" << std::endl;
	}
	file.close();
}

void Map::createPassabilityMap() {
	
	//Setting impassables
	for (unsigned int i = 0; i < Global::worldObjectHandler->impassables.size(); i++) {
		ImpassableWorldObject* tempImpassableObject = Global::worldObjectHandler->impassables[i];
		for (unsigned int j = 0; j < tempImpassableObject->getImpassableTiles().size(); j++) {
			Point absolute = tempImpassableObject->getImpassableTiles()[j] + tempImpassableObject->getPosition();
			
			//in case the object extends beyond the borders of the map
			if (getTile(absolute) == NULL) continue;
			
			getTile(absolute)->setTileInfo(TileInfo::IMPASSABLE);
		}
	}
	
	//Setting interactives
	for (unsigned int i = 0; i < Global::worldObjectHandler->interactives.size(); i++) {
		InteractiveWorldObject* tempInteractiveObject = Global::worldObjectHandler->interactives[i];
		for (unsigned int j = 0; j < tempInteractiveObject->getInteractiveTiles().size(); j++) {
			Point absolute = tempInteractiveObject->getInteractiveTiles()[j] + tempInteractiveObject->getPosition();
			
			//in case the object extends beyond the borders of the map
			if (getTile(absolute) == NULL) continue;
			
			getTile(absolute)->setTileInfo(TileInfo::INTERACTIVE);
			getTile(absolute)->setInterOnTile(tempInteractiveObject);
		}
	}
	
	//Some tiles are impassable eventough we do not place terrain objects on the top of them
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (std::find(baseImpassable.begin(), baseImpassable.end(), getTile(i, j)->getType()) != baseImpassable.end()) {
				getTile(i, j)->setTileInfo(TileInfo::IMPASSABLE);
			}
		}
	}
}

bool Map::isTileVisible(int x, int y) {
	if (Global::camera->getPosition().getX() / Global::tileSize <= x &&
		Global::camera->getPosition().getY() / Global::tileSize <= y &&
		(Global::camera->getPosition().getX() + Global::screenWidth) / Global::tileSize + 1 >= x &&
		(Global::camera->getPosition().getY() + Global::screenHeight) / Global::tileSize + 1 >= y) {
		
		return true;
	}
	return false;
}

bool Map::isTileVisible(Point tPos) {
	return isTileVisible(tPos.getX(), tPos.getY());
}

void Map::renderPassabilityDebugInfo() {
	//Same as the normal tilerendering
	Point startTileCoordinates = Point(Global::camera->getPosition().getX() / Global::tileSize, Global::camera->getPosition().getY() / Global::tileSize);
	Point endTileCoordinates = Point((Global::camera->getPosition().getX() + Global::screenWidth) / Global::tileSize + 1, (Global::camera->getPosition().getY() + Global::screenHeight) / Global::tileSize + 1);
	
	for (int i = startTileCoordinates.getX(); i <= endTileCoordinates.getX(); i++) {
		for (int j = startTileCoordinates.getY(); j <= endTileCoordinates.getY(); j++) {
			if (getTile(i, j) == NULL) {
				continue;
			}
			
			//Setting rectangle
			SDL_Rect destinationRect;
			destinationRect.x = i * Global::tileSize - Global::camera->getPosition().getX();
			destinationRect.y = j * Global::tileSize - Global::camera->getPosition().getY();
			destinationRect.w = Global::tileSize;
			destinationRect.h = Global::tileSize;
			
			//sets the color of the rect
			if (getTile(i, j)->getTileInfo() == TileInfo::FREE) {
				//Green
				SDL_SetRenderDrawColor(Global::renderer, 0x00, 0xDD, 0x00, 0x00);
			} else if (getTile(i, j)->getTileInfo() == TileInfo::IMPASSABLE) {
				//Red
				SDL_SetRenderDrawColor(Global::renderer, 0xDD, 0x00, 0x00, 0xAF);
			} else {
				//Yellow
				SDL_SetRenderDrawColor(Global::renderer, 0xDD, 0xDD, 0x00, 0xAF);
			}
			
			SDL_RenderFillRect(Global::renderer, &destinationRect);
		}
	}
}

void Map::renderTileCoordinates() {
	//Same as the normal tilerendering
	Point startTileCoordinates = Point(Global::camera->getPosition().getX() / Global::tileSize, Global::camera->getPosition().getY() / Global::tileSize);
	Point endTileCoordinates = Point((Global::camera->getPosition().getX() + Global::screenWidth) / Global::tileSize + 1, (Global::camera->getPosition().getY() + Global::screenHeight) / Global::tileSize + 1);
	
	for (int i = startTileCoordinates.getX(); i <= endTileCoordinates.getX(); i++) {
		for (int j = startTileCoordinates.getY(); j <= endTileCoordinates.getY(); j++) {
			if (getTile(i, j) == NULL) {
				continue;
			}
			
			SDL_SetRenderDrawColor(Global::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawLine(Global::renderer,
				i * Global::tileSize - Global::camera->getPosition().getX(),
				Global::permaGUI->getUpperHeight(),
				i * Global::tileSize - Global::camera->getPosition().getX(),
				Global::screenHeight - Global::permaGUI->getLowerHeight()
			);
			
			SDL_RenderDrawLine(Global::renderer,
				0,
				j * Global::tileSize - Global::camera->getPosition().getY(),
				Global::screenWidth,
				j * Global::tileSize - Global::camera->getPosition().getY()
			);
			
			int textSize = Global::tileSize / 4;
			
			ATexture* textTextureX = Global::resourceHandler->getTextTexture(std::to_string(i), Global::resourceHandler->getColor("debug-coord"), textSize);
			Dimension d = textTextureX->getDimensions();
			
			//Setting rectangle
			SDL_Rect destinationRect;
			destinationRect.x = i * Global::tileSize - Global::camera->getPosition().getX() + Global::tileSize / 2 - d.W() / 2;
			destinationRect.y = j * Global::tileSize - Global::camera->getPosition().getY() + Global::tileSize / 4 - d.H() / 2;
			destinationRect.w = d.W();
			destinationRect.h = d.H();
			
			textTextureX->render(destinationRect);
			
			
			
			ATexture* textTextureY = Global::resourceHandler->getTextTexture(std::to_string(j), Global::resourceHandler->getColor("debug-coord"), textSize);
			d = textTextureY->getDimensions();
			
			//Setting rectangle
			destinationRect.x = i * Global::tileSize - Global::camera->getPosition().getX() + Global::tileSize / 2 - d.W() / 2;
			destinationRect.y = j * Global::tileSize - Global::camera->getPosition().getY() + Global::tileSize * 3 / 4 - d.H() / 2;
			destinationRect.w = d.W();
			destinationRect.h = d.H();
			
			textTextureY->render(destinationRect);
		}
	}
}

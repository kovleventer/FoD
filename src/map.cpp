#include "map.h"

#include "global.h"
#include "circularpath.h"

Map::Map(int w, int h) {
	width = w;
	height = h;
	size = w * h;
	
	tileMapPath = "data/map/tilemap.data";
	
	loadTileMap();
	
	//NOTE ugly solution but i could not come up with a better idea
	getTile(Global::player->getPosition())->entities.push_back(Global::player);
	
	allowDebug = false;
}

Map::~Map() {
	//delete tilemap
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//TODO delete all entities on tile
			delete tiles[i][j];
		}
	}
	
	for (int i = 0; i < width; i++) {
		delete tiles[i];
	}
	
	delete tiles;
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
	
	if (allowDebug) {
		renderPassabilityDebugInfo();
	}
}

Point Map::getTileFromCursorPosition(Point cursorPosition) {
	//If we pass an invalid cursor position we return (-1, -1)
	if (cursorPosition.getX() < 0 ||
		cursorPosition.getX() >= Global::screenWidth ||
		cursorPosition.getY() < 0 ||
		cursorPosition.getY() >= Global::screenHeight) {
		return Point(-1, -1);
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

bool Map::getAllowDebug() {
	return allowDebug;
}

void Map::setAllowDebug(bool newAllowDebug) {
	allowDebug = newAllowDebug;
}

void Map::renderTileMap() {
	//We only render visible tiles
	Point startTileCoordinates = Point(Global::camera->getPosition().getX() / Global::tileSize, Global::camera->getPosition().getY() / Global::tileSize);
	Point endTileCoordinates = Point((Global::camera->getPosition().getX() + Global::screenWidth) / Global::tileSize + 1, (Global::camera->getPosition().getY() + Global::screenHeight) / Global::tileSize + 1);
	
	for (int i = startTileCoordinates.getX(); i <= endTileCoordinates.getX(); i++) {
		for (int j = startTileCoordinates.getY(); j <= endTileCoordinates.getY(); j++) {
			//If Tile does not exist, we do not render it
			//NOTE this should be unnecessary later since we will fix the camera position
			if (getTile(i, j) == NULL) {
				continue;
			}
			
			//Setting rectangle
			SDL_Rect destinationRect;
			destinationRect.x = i * Global::tileSize - Global::camera->getPosition().getX();
			destinationRect.y = j * Global::tileSize - Global::camera->getPosition().getY();
			destinationRect.w = Global::tileSize;
			destinationRect.h = Global::tileSize;
			SDL_RenderCopy(Global::renderer, getTile(i, j)->texture, NULL, &destinationRect);
		}
	}
}

void Map::renderMapEntities() {
	//We only render obects on visible tiles
	Point startTileCoordinates = Point(Global::camera->getPosition().getX() / Global::tileSize, Global::camera->getPosition().getY() / Global::tileSize);
	Point endTileCoordinates = Point((Global::camera->getPosition().getX() + Global::screenWidth) / Global::tileSize + 1, (Global::camera->getPosition().getY() + Global::screenHeight) / Global::tileSize + 1);
	
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
						
						//Built in SDL function, to get the width and the height of the texture
						//Params: texture, format, access, w, h
						int w, h;
						SDL_QueryTexture(currEnt->texture, NULL, NULL, &w, &h);
						
						//Setting rectangle
						SDL_Rect destinationRect;
						
						//We are setting the width and the height first
						//Ugly casting becasue scale is a double member
						destinationRect.w = (int)(((double)w * Global::tileSize / 64) * currEnt->getScale());
						destinationRect.h = (int)(((double)h * Global::tileSize / 64) * currEnt->getScale());
						
						//Every rectangle looks like this:
						// 123
						// 4 5
						// 678
						//By default we are fixing the '7' points (down-center)
						//So the texture's down-center point will be at the same position as its tile's
						
						//The top-left corner of its tile
						destinationRect.x = currEnt->getPosition().getX() * Global::tileSize - Global::camera->getPosition().getX();
						destinationRect.y = currEnt->getPosition().getY() * Global::tileSize - Global::camera->getPosition().getY();
						
						//We need to move this point with a vector
						destinationRect.x -= (destinationRect.w - Global::tileSize) / 2;
						destinationRect.y -= destinationRect.h - Global::tileSize;
						
						SDL_RenderCopy(Global::renderer, currEnt->texture, NULL, &destinationRect);
					}
				} else {
					for (unsigned int k = 0; k < current->entities.size(); k++) {
						//currEnt get it?:D
						MapEntity* currEnt = current->entities[k];
						
						//NULL-checking
						if (currEnt == NULL) {
							continue;
						}
						
						//Built in SDL function, to get the width and the height of the texture
						//Params: texture, format, access, w, h
						int w, h;
						SDL_QueryTexture(currEnt->texture, NULL, NULL, &w, &h);
						
						//Setting rectangle
						SDL_Rect destinationRect;
						
						//We are setting the width and the height first
						//Ugly casting becasue scale is a double member
						destinationRect.w = (int)(((double)w * Global::tileSize / 64) * currEnt->getScale());
						destinationRect.h = (int)(((double)h * Global::tileSize / 64) * currEnt->getScale());
						
						//Every rectangle looks like this:
						// 123
						// 4 5
						// 678
						//By default we are fixing the '7' points (down-center)
						//So the texture's down-center point will be at the same position as its tile's
						
						//The top-left corner of its tile
						destinationRect.x = currEnt->getPosition().getX() * Global::tileSize - Global::camera->getPosition().getX();
						destinationRect.y = currEnt->getPosition().getY() * Global::tileSize - Global::camera->getPosition().getY();
						
						//We need to move this point with a vector
						destinationRect.x -= (destinationRect.w - Global::tileSize) / 2;
						destinationRect.y -= destinationRect.h - Global::tileSize;
						
						//If the entity is an npc or a player, we add the progressvector
						if (currEnt == Global::player) {
							destinationRect.x += Global::player->getProgressVector().getX() * Global::tileSize;
							destinationRect.y += Global::player->getProgressVector().getY() * Global::tileSize;
						}
						NPC* possibleNPC = dynamic_cast<NPC*>(currEnt);
						if (possibleNPC != NULL && !possibleNPC->getStanding()) {
							PointD corrigation = (possibleNPC->getPath()->current() - possibleNPC->getPosition()) * Global::tileSize;
							
							destinationRect.x += possibleNPC->getProgressVector().getX() * Global::tileSize;
							destinationRect.x += corrigation.getX();
							destinationRect.y += possibleNPC->getProgressVector().getY() * Global::tileSize;
							destinationRect.y += corrigation.getY();
						}
						
						SDL_RenderCopy(Global::renderer, currEnt->texture, NULL, &destinationRect);
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
		
		SDL_Texture* texture;
		
		if (diff == Point(1, -1)) {
			//Up right
			texture = Global::resourceHandler->pathTextures["upright"];
		} else if (diff == Point(1, 0)) {
			//Right
			texture = Global::resourceHandler->pathTextures["right"];
		} else if (diff == Point(1, 1)) {
			//Down right
			texture = Global::resourceHandler->pathTextures["downright"];
		} else if (diff == Point(0, 1)) {
			//Down
			texture = Global::resourceHandler->pathTextures["down"];
		} else if (diff == Point(-1, 1)) {
			//Down left
			texture = Global::resourceHandler->pathTextures["downleft"];
		} else if (diff == Point(-1, 0)) {
			//Left
			texture = Global::resourceHandler->pathTextures["left"];
		} else if (diff == Point(-1, -1)) {
			//Up left
			texture = Global::resourceHandler->pathTextures["upleft"];
		} else if (diff == Point(0, -1)) {
			//Up
			texture = Global::resourceHandler->pathTextures["up"];
		} else {
			std::cout << "Path rendering bug" << std::endl;
			return;
		}
		
		//Setting renctangle
		destinationRect.x = thisPoint.getX() * Global::tileSize - Global::camera->getPosition().getX();
		destinationRect.y = thisPoint.getY() * Global::tileSize - Global::camera->getPosition().getY();
		SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
	}
	
	//Destination tile
	destinationRect.x = Global::player->getPath()[Global::player->getPath().size() - 1].getX() * Global::tileSize - Global::camera->getPosition().getX();
	destinationRect.y = Global::player->getPath()[Global::player->getPath().size() - 1].getY() * Global::tileSize - Global::camera->getPosition().getY();
	SDL_RenderCopy(Global::renderer, Global::resourceHandler->pathTextures["destination"], NULL, &destinationRect);
}

void Map::loadTileMap() {
	tiles = new Tile**[width];
	for (int i = 0; i < width; ++i) {
		tiles[i] = new Tile*[height];
	}
	
	std::fstream file;
	//NOTE not the best way to handle maps
	//Well, I do not know any better way
	file.open(tileMapPath, std::ios::in);
	
	//Initializing the contents of the 2D array
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int tempType;
			file >> tempType;
			tiles[j][i] = new Tile(tempType);
		}
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
			
			getTile(absolute)->setTileInfo(TileInfo::FRIENDLY);
		}
	}
	
	//Some tiles are impassable eventough we do not place terrain objects on the top of them
	std::vector<std::string> baseImpassable = {"water"};
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

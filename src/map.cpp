#include "map.h"

#include "global.h"
#include "circularpath.h"

Map::Map(int w, int h) {
	width = w;
	height = h;
	size = w * h;
	
	tileMapPath = "data/map/tilemap.data";
	terrainMapPath = "data/map/objects.data";
	interactivePath = "data/map/interactive.data";
	npcPath = "data/map/npcs.data";
	
	loadTileMap();
	loadMapEntities();
	createPassabilityMap();
}

Map::~Map() {
	for (unsigned int i = 0; i < mapEntities.size(); i++) {
		delete mapEntities[i];
	}
	mapEntities.clear();
	
	//delete tilemap
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
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
	
	//TODO better debug handling
	//renderPassabilityDebugInfo();
}

void Map::updateNPCsPosition() {
	for (unsigned int i = 0; i < npcs.size(); i++) {
		npcs[i]->updateNPCPosition();
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

NPC* Map::getNPC(int index) {
	return npcs[index];
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
	
	for (int i = startTileCoordinates.getY(); i <= endTileCoordinates.getY(); i++) {
		for (int j = startTileCoordinates.getX(); j <= endTileCoordinates.getX(); j++) {
			Tile* current = getTile(j, i);
			
			//If Tile does not exist, we do not render it
			//NOTE this should be unnecessary later since we will fix the camera position
			if (current == NULL) {
				continue;
			}
			
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
				
				//We are setting the width and the hight first
				destinationRect.w = w / 64 * Global::tileSize;
				destinationRect.h = h / 64 * Global::tileSize;
				
				//Every rectangle looks like this:
				// 123
				// 4 5
				// 678
				//By default wer are fixing the '7' points (down-center)
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
				if (possibleNPC != NULL) {
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

void Map::renderPath() {
	SDL_Rect destinationRect;
	destinationRect.w = Global::tileSize;
	destinationRect.h = Global::tileSize;
	for (unsigned int i = Global::player->getTileProgress() == 0 ? 1 : Global::player->getTileProgress(); i < Global::player->getPath().size()-1; i++) {
		
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

void Map::loadMapEntities() {
	//ALERT unsafe, might cause an infinite loop
	std::fstream file;
	
	
	//Terrain
	file.open(terrainMapPath, std::ios::in);
	//texture file pattern:
	//a b c d ['e f' d times]
	//a: the ID of the texture (see reshandler)
	//b: the x coordinate of its tile
	//c: the y coordinate of its tile
	//d: the number of the impassable tiles
	//e: one impassable tile's x coordinate
	//f: one impassable tile's y coordinate
	while (true) {
		
		int textID, tileX, tileY, impLen;
		file >> textID;
		file >> tileX;
		file >> tileY;
		file >> impLen;
		std::vector<Point> impassableTiles;
		
		//NOTE this is ugly as fuck, but somehow it works this way
		if (file.eof()) break;
		
		
		for (int i = 0; i < impLen; i++) {
			int x, y;
			file >> x;
			file >> y;
			impassableTiles.push_back(Point(x, y));
		}
		MapEntity* loaded = new WorldObject(textID, Point(tileX, tileY), impassableTiles);
		mapEntities.push_back(loaded);
		tiles[tileX][tileY]->entities.push_back(loaded);
	}
	file.close();
	
	
	//Interactives
	file.open(interactivePath, std::ios::in);
	//texture file pattern:
	//a b c d ['e f' d times]
	//a: the ID of the texture (see reshandler)
	//b: the x coordinate of its tile
	//c: the y coordinate of its tile
	//d: the number of the interactive tiles
	//e: one impassable tile's x coordinate
	//f: one impassable tile's y coordinate
	while (true) {
		
		int textID, tileX, tileY, interLen;
		file >> textID;
		file >> tileX;
		file >> tileY;
		file >> interLen;
		std::vector<Point> interactiveTiles;
		
		//NOTE this is ugly as fuck, but somehow it works this way
		if (file.eof()) break;
		
		
		for (int i = 0; i < interLen; i++) {
			int x, y;
			file >> x;
			file >> y;
			interactiveTiles.push_back(Point(x, y));
		}
		MapEntity* loaded = new InteractiveWorldObject(textID, Point(tileX, tileY), interactiveTiles);
		mapEntities.push_back(loaded);
		tiles[tileX][tileY]->entities.push_back(loaded);
	}
	file.close();
	
	
	//NPCs
	file.open(npcPath, std::ios::in);
	//npc file pattern:
	//a b ['c d' b times]
	//a: the ID of the texture (see reshandler)
	//b: the number of the notable parts of the map
	//c: the y coordinate of its tile
	while (true) {
		int textID, pathLen;
		file >> textID;
		file >> pathLen;
		std::vector<Point> pathPieces;
		
		//NOTE this is ugly as fuck, but somehow it works this way
		if (file.eof()) break;
		
		for (int i = 0; i < pathLen; i++) {
			int x, y;
			file >> x;
			file >> y;
			pathPieces.push_back(Point(x, y));
		}
		
		//NOTE a little bit of redundancy here
		//MapEntity* loaded;
		NPC* loaded;
		if (pathPieces.size() == 1) {
			loaded = new NPC(textID, pathPieces[0]);
			tiles[pathPieces[0].getX()][pathPieces[0].getY()]->entities.push_back(loaded);
		} else if (pathPieces.size() == 0) {
			loaded = new NPC(textID, Point(0, 0));
			tiles[0][0]->entities.push_back(loaded);
		} else {
			loaded = new NPC(textID, pathPieces);
			tiles[pathPieces[0].getX()][pathPieces[0].getY()]->entities.push_back(loaded);
		}
		
		mapEntities.push_back(loaded);
		npcs.push_back(loaded);
	}
	file.close();
	
	
	//Player
	tiles[Global::player->getPosition().getX()][Global::player->getPosition().getY()]->entities.push_back(Global::player);
}

void Map::createPassabilityMap() {
	for (unsigned int i = 0; i < mapEntities.size(); i++) {
		WorldObject* tempWorldObject = dynamic_cast<WorldObject*>(mapEntities[i]);
		if (tempWorldObject != NULL) {
			for (unsigned int j = 0; j < tempWorldObject->getImpassableTiles().size(); j++) {
				int newX = tempWorldObject->getImpassableTiles()[j].getX() + tempWorldObject->getPosition().getX();
				int newY = tempWorldObject->getImpassableTiles()[j].getY() + tempWorldObject->getPosition().getY();
				
				//in case the object extends beyond the borders of the map
				if (getTile(newX, newY) == NULL) continue;
				
				getTile(newX, newY)->setTileInfo(TileInfo::IMPASSABLE);
			}
		} else {
			InteractiveWorldObject* tempInteractiveObject = dynamic_cast<InteractiveWorldObject*>(mapEntities[i]);
			if (tempInteractiveObject != NULL) {
				for (unsigned int j = 0; j < tempInteractiveObject->getInteractiveTiles().size(); j++) {
					int newX = tempInteractiveObject->getInteractiveTiles()[j].getX() + tempInteractiveObject->getPosition().getX();
					int newY = tempInteractiveObject->getInteractiveTiles()[j].getY() + tempInteractiveObject->getPosition().getY();
					
					//in case the object extends beyond the borders of the map
					if (getTile(newX, newY) == NULL) continue;
					
					getTile(newX, newY)->setTileInfo(TileInfo::FRIENDLY);
				}
			}
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

void Map::createNPCPath() {
	for (unsigned int i = 0; i < npcs.size(); i++) {
		NPC* npc = npcs[i];
		if (!npc->getStanding()) {
			npc->setPath(new CircularPath(npc->getTempCont()));
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

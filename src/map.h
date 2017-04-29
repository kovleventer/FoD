#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <typeinfo>

#include "tile.h"
#include "point.h"
#include "worldobject.h"
#include "interactiveworldobject.h"
#include "player.h"
#include "npc.h"
#include "mapentity.h"

class Map {
public:
	Map(int w, int h);
	~Map();
	
	//NOTE init stuff that must run after th constructor has finished its job
	//generates the npc's paths
	void createNPCPath();
	
	//Getters
	int getWidth();
	int getHeight();
	int getSize();
	
	Tile* getTile(int x, int y);
	Tile* getTile(Point pos);
	
	void render();
	
	void updateNPCsPosition();
	
	//Converts screen coordinates to tile coordinates
	Point getTileFromCursorPosition(Point cursorPosition);
	
	//Gets an NPC
	//Returns NULL, when there aren't any npcs standing on the tile
	NPC* getNPCOnTile(Point tilePos);
	NPC* getNPCOnTile(Tile* tile);
private:
	int width;
	int height;
	//Number of tiles
	int size;
	
	//This is a two-dimensional array of Tile pointers
	Tile*** tiles;
	
	//We are storing the map objects in vectors
	std::vector<MapEntity*> mapEntities;
	
	//We need to store the NPC's in its own vector too, since we are going to traverse through them frequently
	std::vector<NPC*> npcs;
	
	//Rendering
	void renderTileMap();
	void renderMapEntities();
	void renderPlayer();
	void renderPath();
	
	//Loading
	//Uses file IO
	void loadTileMap();
	void loadMapEntities();
	//sets the passablitity of the tiles
	void createPassabilityMap();
	
	std::string tileMapPath;
	std::string terrainMapPath;
	std::string interactivePath;
	std::string npcPath;
	
	//Utility functions
	bool isTileVisible(int x, int y);
	bool isTileVisible(Point tPos);
	
	//DEBUG
	//TODO add transparency
	void renderPassabilityDebugInfo();
};

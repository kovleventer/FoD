#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <vector>

#include "mapentity.h"
#include "animatabletexture.h"

enum class TileInfo {
	FREE,
	IMPASSABLE,
	FRIENDLY
};

class Tile {
public:
	Tile(std::string tileType);
	
	//For tilemap (First stage) rendering
	ATexture* texture;
	
	//Second rendering stage objects
	std::vector<MapEntity*> backgroundEntities;
	//Third rendering stage objects
	std::vector<MapEntity*> entities;
	
	//Getters
	std::string getType();
	TileInfo getTileInfo();
	
	//Setters
	void setTileInfo(TileInfo newTileInfo);
private:
	std::string type;
	
	//Is it passable or impassable? Does it have a building on it?
	TileInfo tileInfo;
};

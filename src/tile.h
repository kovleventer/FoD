#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <vector>

#include "mapentity.h"

enum class TileInfo {
	FREE,
	IMPASSABLE,
	FRIENDLY
};

class Tile {
public:
	Tile();
	Tile(std::string tileType);
	Tile(int tileType);
	
	//For tilemap (First stage) rendering
	SDL_Texture* texture;
	
	//Second rendering stage objects
	std::vector<MapEntity*> backgroundEntities;
	//Third rendering stage objects
	std::vector<MapEntity*> entities;
	
	//The default tile type
	static const std::string DEFAULT_TYPE;
	
	//Getters and setters
	std::string getType();
	TileInfo getTileInfo();
	void setTileInfo(TileInfo newTileInfo);
private:
	std::string type;
	
	//Is it passable or impassable? Does it have a building on it?
	TileInfo tileInfo;
	
	//To support multiple constructors
	void init(std::string tileType);
};

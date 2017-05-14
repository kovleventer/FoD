#pragma once

#include <string>
#include <SDL.h>
#include <vector>

#include "mapentity.h"
#include "../core/animatabletexture.h"
#include "worldobject.h"

/*!
 * @author kovlev
 */


/*!
 * @enum TileInfo helps determinating the cursor texture when hovered over this tile
 */
enum class TileInfo {
	FREE,
	IMPASSABLE,
	INTERACTIVE
};


/*!
 * @class Tile the most basic layer of the map
 * We have a lot of them so they should be lightweight
 */
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
	InteractiveWorldObject* getInterOnTile();
	
	//Setters
	void setTileInfo(TileInfo newTileInfo);
	void setInterOnTile(InteractiveWorldObject* newInterOnTile);
private:
	//The tile's type
	std::string type;
	
	//Is it passable or impassable? Does it have a building on it?
	TileInfo tileInfo;
	
	//This is needed since the interactives may have more activatable tiles
	//And we only want to store them in the entities list once
	InteractiveWorldObject* interOnTile;
};

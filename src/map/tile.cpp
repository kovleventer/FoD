#include "tile.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

Tile::Tile(std::string tileType) {
	type = tileType;
	tileInfo = TileInfo::FREE;
	
	//Every tile has a pointer to its texture
	texture = Global::resourceHandler->getATexture(TT::TILE, type);
	
	interOnTile = NULL;
}

std::string Tile::getType() {
	return type;
}

TileInfo Tile::getTileInfo() {
	return tileInfo;
}

InteractiveWorldObject* Tile::getInterOnTile() {
	return interOnTile;
}

void Tile::setTileInfo(TileInfo newTileInfo) {
	tileInfo = newTileInfo;
}

void Tile::setInterOnTile(InteractiveWorldObject* newInterOnTile) {
	interOnTile = newInterOnTile;
}

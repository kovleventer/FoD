#include "tile.h"

#include "global.h"

Tile::Tile(std::string tileType) {
	type = tileType;
	tileInfo = TileInfo::FREE;
	
	//Every tile has a pointer to its texture
	texture = Global::resourceHandler->getATexture(TT::TILE, type);
}

std::string Tile::getType() {
	return type;
}

TileInfo Tile::getTileInfo() {
	return tileInfo;
}

void Tile::setTileInfo(TileInfo newTileInfo) {
	tileInfo = newTileInfo;
}

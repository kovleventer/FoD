#include "tile.h"

#include "global.h"

const std::string Tile::DEFAULT_TYPE = "grass";

Tile::Tile() {
	init(DEFAULT_TYPE);
}

Tile::Tile(std::string tileType) {
	init(tileType);
}

Tile::Tile(int tileType) {
	init(Global::resourceHandler->tileTextureIDs[tileType]);
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

void Tile::init(std::string tileType) {
	type = tileType;
	tileInfo = TileInfo::FREE;
	
	//Every tile has a pointer to its texture
	texture = Global::resourceHandler->tileTextures[type];
}

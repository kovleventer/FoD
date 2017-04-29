#include "mapentity.h"

#include "tile.h"
#include "global.h"

MapEntity::MapEntity(Point pos) {
	position = pos;
}

MapEntity::~MapEntity() {}

Point MapEntity::getPosition() {
	return position;
}

void MapEntity::setPosition(Point newPosition) {
	//We get the tiles
	Tile* thisTile = Global::map->getTile(position);
	Tile* nextTile = Global::map->getTile(newPosition);
	
	//If the tile is incorrect
	if (nextTile == NULL) return;
	
	//We move the pointers
	thisTile->entities.erase(std::remove(thisTile->entities.begin(), thisTile->entities.end(), this));
	nextTile->entities.push_back(this);
	
	position = newPosition;
}

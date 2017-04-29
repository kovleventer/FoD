#include "mapentity.h"

#include "tile.h"
#include "global.h"

//Default scale is 1
MapEntity::MapEntity(int x, int y) : MapEntity(Point(x, y), 1) {}

MapEntity::MapEntity(Point pos) : MapEntity(pos, 1) {}

MapEntity::MapEntity(int x, int y, double s) : MapEntity(Point(x, y), s) {}

MapEntity::MapEntity(Point pos, double s) {
	position = pos;
	scale = s;
}

MapEntity::~MapEntity() {
	if (Global::map != NULL) {
		Tile* thisTile = Global::map->getTile(position);
		if (thisTile != NULL) {
			if (atBackground) {
				thisTile->backgroundEntities.erase(std::remove(thisTile->backgroundEntities.begin(), thisTile->backgroundEntities.end(), this));
			} else {
				thisTile->entities.erase(std::remove(thisTile->entities.begin(), thisTile->entities.end(), this));
			}
		}
	}
}

Point MapEntity::getPosition() {
	return position;
}

double MapEntity::getScale() {
	return scale;
}

bool MapEntity::isAtBackground() {
	return atBackground;
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

void MapEntity::setScale(double newScale) {
	scale = newScale;
}

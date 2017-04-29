#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "point.h"
#include "mapentity.h"

class WorldObject : public MapEntity {
public:
	WorldObject(int textID, Point tile, std::vector<Point> imp);
	
	//Getters
	std::vector<Point> getImpassableTiles();
private:
	//Impassable tiles are relative(0, 0) means the positionTile
	std::vector<Point> impassableTiles;
};

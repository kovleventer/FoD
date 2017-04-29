#pragma once

#include <string>
#include <vector>

#include "worldobject.h"
#include "point.h"

class ImpassableWorldObject : public WorldObject {
public:
	ImpassableWorldObject(std::string text, int x, int y, std::vector<Point> impTiles);
	ImpassableWorldObject(std::string text, Point pos, std::vector<Point> impTiles);
	
	//Getters
	std::vector<Point> getImpassableTiles();
private:
	std::vector<Point> impassableTiles;
};
